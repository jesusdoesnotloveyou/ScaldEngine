#include "../../ScaldException.h"
#include "ShadowMap.h"

ShadowMap::ShadowMap(ID3D11Device* device, UINT width, UINT height)
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
	// Use typeless format because the DSV is going to interpret the bits as DXGI_FORMAT_D24_UNORM_S8_UINT, whereas the SRV is going 
	// to interpret the bits as DXGI_FORMAT_R24_UNORM_X8_TYPELESS.
	D3D11_TEXTURE2D_DESC shadowMapDesc;
	shadowMapDesc.Width = mWidth;
	shadowMapDesc.Height = mHeight;
	shadowMapDesc.MipLevels = 1u;
	shadowMapDesc.ArraySize = 1u;
	shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	shadowMapDesc.SampleDesc.Count = 1u;
	shadowMapDesc.SampleDesc.Quality = 0u;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowMapDesc.CPUAccessFlags = 0u;
	shadowMapDesc.MiscFlags = 0u;

	ID3D11Texture2D* depthMap = nullptr;
	ThrowIfFailed(device->CreateTexture2D(&shadowMapDesc, nullptr, &depthMap));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Flags = 0u;
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;
	ThrowIfFailed(device->CreateDepthStencilView(depthMap, &depthStencilViewDesc, &mDepthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = shadowMapDesc.MipLevels;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0u;
	ThrowIfFailed(device->CreateShaderResourceView(depthMap, &shaderResourceViewDesc, &mDepthMapSRV));

	// probably should be created in Graphics, not in ShadowMap
	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.0f;
	comparisonSamplerDesc.BorderColor[1] = 1.0f;
	comparisonSamplerDesc.BorderColor[2] = 1.0f;
	comparisonSamplerDesc.BorderColor[3] = 1.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0u;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

	depthMap->Release();
}

ShadowMap::~ShadowMap() noexcept
{
	mDepthMapDSV->Release();
	mDepthMapSRV->Release();
}

ID3D11ShaderResourceView** ShadowMap::GetDepthMapSRV()
{
	return &mDepthMapSRV;
}

void ShadowMap::BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc)
{
	dc->RSSetViewports(1, &mViewport);
	// Set null render target because we are only going to draw to depth buffer. Setting a null render target will disable color writes.
	ID3D11RenderTargetView* renderTarget = nullptr;
	dc->OMSetRenderTargets(1u, &renderTarget, mDepthMapDSV);
	dc->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}