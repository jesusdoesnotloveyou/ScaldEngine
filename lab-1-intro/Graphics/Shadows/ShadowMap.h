#pragma once

#include "d3d11.h"

class ShadowMap
{
public:
	ShadowMap(ID3D11Device* device, UINT width, UINT height);
	~ShadowMap() noexcept;
	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);

private:
	ShadowMap(const ShadowMap& lhs);
	ShadowMap& operator=(const ShadowMap& lhs) = default;

private:
	UINT mWidth;
	UINT mHeight;
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
};