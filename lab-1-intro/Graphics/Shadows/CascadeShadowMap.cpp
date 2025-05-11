#include "../../ScaldException.h"
#include "CascadeShadowMap.h"

CascadeShadowMap::CascadeShadowMap(ID3D11Device* device, UINT width, UINT height)
	:
	mWidth(width),
	mHeight(height)
{
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


	ID3D11Texture2D* depthMapsArray = nullptr;
	ThrowIfFailed(device->CreateTexture2D(&depthDescription, nullptr, &depthMapsArray));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Flags = 0u;
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthStencilViewDesc.Texture2DArray.MipSlice = 0u;
	depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0u;
	depthStencilViewDesc.Texture2DArray.ArraySize = CASCADE_NUMBER;
	ThrowIfFailed(device->CreateDepthStencilView(depthMapsArray, &depthStencilViewDesc, &mDepthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0u;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1u;
	shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0u;
	shaderResourceViewDesc.Texture2DArray.ArraySize = CASCADE_NUMBER;
	ThrowIfFailed(device->CreateShaderResourceView(depthMapsArray, &shaderResourceViewDesc, &mDepthMapSRV));

	depthMapsArray->Release();
}
