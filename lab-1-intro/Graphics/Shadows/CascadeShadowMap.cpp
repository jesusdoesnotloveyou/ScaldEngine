#include "../../ScaldException.h"
#include "CascadeShadowMap.h"

CascadeShadowMap::CascadeShadowMap(ID3D11Device* device, UINT width, UINT height)
	:
	mWidth(width),
	mHeight(height),
	mDepthMapSRV(nullptr),
	mDepthMapDSV(nullptr)
{
	// Viewport that matches the shadow map dimensions.
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = static_cast<float>(width);
	mViewport.Height = static_cast<float>(height);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	D3D11_TEXTURE2D_DESC depthDescription = {};
	depthDescription.Width = width;
	depthDescription.Height = height;
	depthDescription.MipLevels = 1u;
    depthDescription.ArraySize = CASCADE_NUMBER;
	depthDescription.Format = DXGI_FORMAT_R32_TYPELESS;
	depthDescription.SampleDesc.Count = 1u;
	depthDescription.SampleDesc.Quality = 0u;
	depthDescription.Usage = D3D11_USAGE_DEFAULT;
	depthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthDescription.CPUAccessFlags = 0u;
	depthDescription.MiscFlags = 0u;

	ID3D11Texture2D* depthMapArray = nullptr;
	ThrowIfFailed(device->CreateTexture2D(&depthDescription, nullptr, &depthMapArray));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Flags = 0u;
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthStencilViewDesc.Texture2DArray.MipSlice = 0u;
	depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0u;
	depthStencilViewDesc.Texture2DArray.ArraySize = CASCADE_NUMBER;
	ThrowIfFailed(device->CreateDepthStencilView(depthMapArray, &depthStencilViewDesc, &mDepthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0u;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1u;
	shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0u;
	shaderResourceViewDesc.Texture2DArray.ArraySize = CASCADE_NUMBER;
	ThrowIfFailed(device->CreateShaderResourceView(depthMapArray, &shaderResourceViewDesc, &mDepthMapSRV));

	depthMapArray->Release();
}

CascadeShadowMap::~CascadeShadowMap() noexcept
{
	mDepthMapSRV->Release();
	mDepthMapDSV->Release();
}

ID3D11ShaderResourceView* CascadeShadowMap::Get() const
{
	return mDepthMapSRV;
}

ID3D11ShaderResourceView* const* CascadeShadowMap::GetAddressOf() const
{
	return &mDepthMapSRV;
}

void CascadeShadowMap::BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetViewports(1, &mViewport);
	// Set null render target because we are only going to draw to depth buffer. Setting a null render target will disable color writes.
	ID3D11RenderTargetView* rtv = nullptr;
	deviceContext->OMSetRenderTargets(1u, &rtv, mDepthMapDSV);
	deviceContext->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}