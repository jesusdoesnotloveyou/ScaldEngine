#include "Renderer.h"
#include "../ScaldCoreTypes.h"
#include "../Shadows/CascadeShadowMap.h"

Renderer::Renderer(IDXGISwapChain* spawChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, int width, int height)
	:
	mDevice(device),
	mDeviceContext(deviceContext),
	mScreenWidth(width),
	mScreenHeight(height)
{
	// Step 03: Get the BackBuffer and create RTV
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackTex;
	// gain access to texture subresource in swap chain (back buffer)
	// check MSDN for more info about GetAddressOf, Get, (&) ReleaseAndGetAddressOf
	ThrowIfFailed(spawChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackTex));
	ThrowIfFailed(mDevice->CreateRenderTargetView(pBackTex.Get(), nullptr, &mRTV));

	// Describe our Depth/Stencil Buffer
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilTextureDesc = {};
	ZeroMemory(&depthStencilTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
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
	ThrowIfFailed(device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &mDSV));

	depthStencilBuffer->Release();
}

void Renderer::SetupShaders()
{
	D3D11_INPUT_ELEMENT_DESC inputLayoutShadowDesc[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0u,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0u,
			0u,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u}
	};

	ThrowIfFailed(mShadowVertexShader.Init(mDevice, inputLayoutShadowDesc, (UINT)std::size(inputLayoutShadowDesc), L"./Shaders/ShadowVertexShader.hlsl"));
	ThrowIfFailed(mCSMGeometryShader.Init(mDevice, L"./Shaders/CSMGeometryShader.hlsl"));
}

void Renderer::CreateDepthStencilState()
{
	//Create depth stencil state
	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(CD3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	ThrowIfFailed(mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf()));
}

void Renderer::CreateRasterizerState()
{
	// Step 10: Setup Rasterizer Stage and Viewport
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(mScreenWidth);
	mViewport.Height = static_cast<float>(mScreenHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FrontCounterClockwise = false;
	ThrowIfFailed(mDevice->CreateRasterizerState(&rastDesc, mRasterizerState.GetAddressOf()));
}

void Renderer::CreateSamplerState()
{
	D3D11_SAMPLER_DESC sampDesc = {};
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ThrowIfFailed(mDevice->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf()));

	D3D11_SAMPLER_DESC shadowSampDesc = {};
	ZeroMemory(&shadowSampDesc, sizeof(shadowSampDesc));
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSampDesc.MipLODBias = 0.0f;
	shadowSampDesc.MaxAnisotropy = 1u;
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	shadowSampDesc.MinLOD = 0.0f;
	shadowSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ThrowIfFailed(mDevice->CreateSamplerState(&shadowSampDesc, mShadowSamplerState.GetAddressOf()));
}

void Renderer::ClearBuffer(float r)
{
	//float color[] = { r, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRTV.Get(), Colors::Black);
	mDeviceContext->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL /*Clearing Both Depth and Stencil*/, 1.0f, 0u);
}

void Renderer::BindDepthOnlyPass()
{
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(mShadowVertexShader.GetInputLayout());

	mDeviceContext->VSSetShader(mShadowVertexShader.Get(), nullptr, 0u);
	mDeviceContext->GSSetShader(mCSMGeometryShader.Get(), nullptr, 0u);
	mDeviceContext->PSSetShader(nullptr, nullptr, 0u);
}