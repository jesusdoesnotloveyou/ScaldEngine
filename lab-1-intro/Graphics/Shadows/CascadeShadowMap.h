#pragma once

#include "ShadowMap.h"

const UINT CASCADE_NUMBER = 4;

class CascadeShadowMap
{
public:
	CascadeShadowMap(ID3D11Device* device, UINT width, UINT height);
	~CascadeShadowMap() noexcept;
	ID3D11ShaderResourceView* Get()const;
	ID3D11ShaderResourceView* const* GetAddressOf()const;
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);
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

	ShadowMap* mCascades[CASCADE_NUMBER];
};