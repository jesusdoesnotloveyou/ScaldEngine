#pragma once

#include "d3d11.h"
#include "../ScaldCoreTypes.h"

class CascadeShadowMap
{
public:
	CascadeShadowMap(ID3D11Device* device, UINT width, UINT height);
	~CascadeShadowMap() noexcept;
	ID3D11ShaderResourceView* Get()const;
	ID3D11ShaderResourceView* const* GetAddressOf()const;
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* deviceContext);
private:
	CascadeShadowMap(const CascadeShadowMap& lhs);
	CascadeShadowMap& operator=(const CascadeShadowMap& lhs) = default;

private:
	// shadow map resolution
	UINT mWidth = 2048;
	UINT mHeight = 2048;

	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
};