#include "DeferredRenderer.h"
#include "../ScaldCoreTypes.h"
#include "../Mesh.h"

#include <vector>

DeferredRenderer::DeferredRenderer(IDXGISwapChain* spawChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
	: Renderer(spawChain, device, deviceContext, width, height)
{
	// See Step 03 in Graphics -> same logic, but in this case there is a number of rtvs
	D3D11_TEXTURE2D_DESC textureDesc = {};
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;
	textureDesc.ArraySize = 1u;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0u;
	textureDesc.MiscFlags = 0u;

	// back buffer I suppose
	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &mGBuffer[i].texture));
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	ZeroMemory(&textureDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		ThrowIfFailed(device->CreateRenderTargetView(mGBuffer[i].texture, &renderTargetViewDesc, &mGBuffer[i].rtv));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	ZeroMemory(&textureDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1u;

	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		ThrowIfFailed(device->CreateShaderResourceView(mGBuffer[i].texture, &shaderResourceViewDesc, &mGBuffer[i].srv));
	}

	std::vector<VertexTex> quadVertices = { VertexTex(), VertexTex(), VertexTex(), VertexTex() };
	std::vector<DWORD> quadIndeces = { 0 }; // at least one due to throwing exception in Init
	screenQuad = new Mesh(device, deviceContext, quadVertices, quadIndeces);
}

DeferredRenderer::~DeferredRenderer() noexcept
{
	for (UINT i = 0; i < BUFFER_COUNT; i++)
	{
		mGBuffer[i].texture->Release();
		mGBuffer[i].rtv->Release();
		mGBuffer[i].srv->Release();
	}
}

void DeferredRenderer::SetupShaders()
{
	Renderer::SetupShaders();

	D3D11_INPUT_ELEMENT_DESC inputLayoutOpaqueDesc[] = {
	D3D11_INPUT_ELEMENT_DESC {
		"POSITION",
		0u,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0u,
		0u,
		D3D11_INPUT_PER_VERTEX_DATA,
		0u},
	D3D11_INPUT_ELEMENT_DESC {
		"TEXCOORD",
		0u,
		DXGI_FORMAT_R32G32_FLOAT,
		0u,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0u},
	D3D11_INPUT_ELEMENT_DESC {
		"NORMAL",
		0u,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0u,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0u}
	};

	D3D11_INPUT_ELEMENT_DESC inputLayoutLightingDesc[] = {
	D3D11_INPUT_ELEMENT_DESC {
		"POSITION",
		0u,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0u,
		0u,
		D3D11_INPUT_PER_VERTEX_DATA,
		0u},
	D3D11_INPUT_ELEMENT_DESC {
		"TEXCOORD",
		0u,
		DXGI_FORMAT_R32G32_FLOAT,
		0u,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0u}
	};

	mOpaqueVertexShader.Init(mDevice, inputLayoutOpaqueDesc, (UINT)std::size(inputLayoutOpaqueDesc), L"./Shaders/DeferredOpaqueVS.hlsl");
	mOpaquePixelShader.Init(mDevice, L"./Shaders/DeferredOpaquePS.hlsl");

	mLightingVertexShader.Init(mDevice, inputLayoutLightingDesc, (UINT)std::size(inputLayoutLightingDesc), L"./Shaders/DeferredLightingVS.hlsl");
	mLightingPixelShader.Init(mDevice, L"./Shaders/DeferredLightingPS.hlsl");
}

void DeferredRenderer::BindGeometryPass()
{
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(mOpaqueVertexShader.GetInputLayout());

	ID3D11RenderTargetView* rtvs[] =
	{
		mGBuffer[0].rtv,	// diffuse
		mGBuffer[1].rtv,	// specular
		mGBuffer[2].rtv,	// normals
	};

	// See Step 11
	mDeviceContext->OMSetRenderTargets(BUFFER_COUNT, rtvs, mDSV.Get());
	// See ClearBuffer
	mDeviceContext->ClearRenderTargetView(mGBuffer[0].rtv, Colors::LightSteelBlue);
	mDeviceContext->ClearRenderTargetView(mGBuffer[1].rtv, Colors::Black);
	mDeviceContext->ClearRenderTargetView(mGBuffer[2].rtv, Colors::Black);
	//mDeviceContext->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);

	mDeviceContext->VSSetShader(mOpaqueVertexShader.Get(), nullptr, 0u);

	mDeviceContext->RSSetViewports(1u, &mViewport);
	mDeviceContext->RSSetState(mRasterizerState.Get());

	// Step 09: Set Pixel Shaders
	mDeviceContext->PSSetShader(mOpaquePixelShader.Get(), nullptr, 0u);
	mDeviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());
}

void DeferredRenderer::BindLightingPass()
{
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // Quad -> two triangles
	mDeviceContext->IASetInputLayout(mLightingVertexShader.GetInputLayout());

	mDeviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), mDSV.Get());
	ClearBuffer(.0f); // to make separate pass in RenderDoc

	mDeviceContext->VSSetShader(mLightingVertexShader.Get(), nullptr, 0u);

	mDeviceContext->RSSetViewports(1u, &mViewport);
	mDeviceContext->RSSetState(mRasterizerState.Get());

	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0u);

	mDeviceContext->PSSetShader(mLightingPixelShader.Get(), nullptr, 0u);
	// Bind GBuffer resources
	mDeviceContext->PSSetShaderResources(0u, 1u, &mGBuffer[0].srv);
	mDeviceContext->PSSetShaderResources(1u, 1u, &mGBuffer[1].srv);
	mDeviceContext->PSSetShaderResources(2u, 1u, &mGBuffer[2].srv);
	
	//mDeviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());
	mDeviceContext->PSSetSamplers(0u, 1u, mShadowSamplerState.GetAddressOf());
	DrawScreenQuad();
}

void DeferredRenderer::BindTransparentPass()
{

}

void DeferredRenderer::DrawScreenQuad()
{
	mDeviceContext->IASetVertexBuffers(0u, 1u, screenQuad->GetVertexBuffer().GetAddressOf(), screenQuad->GetVertexBuffer().GetStridePtr(), screenQuad->GetVertexBuffer().GetOffsetPtr());
	mDeviceContext->IASetIndexBuffer(screenQuad->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
	mDeviceContext->Draw(screenQuad->GetVertexBuffer().GetBufferSize(), 0u);
}