#include "../../ScaldException.h"
#include "DeferredRenderer.h"
#include "../ScaldCoreTypes.h"

DeferredRenderer::DeferredRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
	: Renderer(device, deviceContext, width, height)
{
	// See Step 03 in Graphics -> same logic, but in that case a number of rtv

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

	ID3D11Texture2D* depthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilTextureDesc = {};
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilTextureDesc.Width = width;
	depthStencilTextureDesc.Height = height;
	depthStencilTextureDesc.MipLevels = 1u;
	depthStencilTextureDesc.ArraySize = 1u;
	depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTextureDesc.SampleDesc.Count = 1u;
	depthStencilTextureDesc.SampleDesc.Quality = 0u;
	depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDesc.CPUAccessFlags = 0u;
	depthStencilTextureDesc.MiscFlags = 0u;
	ThrowIfFailed(device->CreateTexture2D(&depthStencilTextureDesc, nullptr, &depthStencilBuffer));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = depthStencilTextureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0u;
	ThrowIfFailed(device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView));

	depthStencilBuffer->Release();
}

DeferredRenderer::~DeferredRenderer() noexcept
{
	mDepthStencilView->Release();
}

void DeferredRenderer::BindGeometryPass()
{
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->RSSetViewports(1u, &mViewport);

	ID3D11RenderTargetView* rtvs[] =
	{
		mGBuffer[0].rtv,	// diffuse
		mGBuffer[1].rtv,	// specular
		mGBuffer[2].rtv,	// normals
	};

	// See Step 11
	mDeviceContext->OMSetRenderTargets(BUFFER_COUNT, rtvs, mDepthStencilView);
	// See ClearBuffer
	mDeviceContext->ClearRenderTargetView(mGBuffer[0].rtv, Colors::LightSteelBlue);
	mDeviceContext->ClearRenderTargetView(mGBuffer[1].rtv, Colors::Black);
	mDeviceContext->ClearRenderTargetView(mGBuffer[2].rtv, Colors::Black);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	

}

void DeferredRenderer::BindLightingPass()
{

}

void DeferredRenderer::BindTransparentPass()
{

}

void DeferredRenderer::Draw()
{
	/*for (auto renderObject : mRenderObjects)
	{

	}*/
}