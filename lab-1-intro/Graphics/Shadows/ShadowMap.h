#pragma once

#include "d3d11.h"

class ShadowMap
{
public:
	ShadowMap(ID3D11Device* device, UINT width, UINT height);
	~ShadowMap() noexcept;
	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);

	void Setup();
private:
	ShadowMap(const ShadowMap& lhs);
	ShadowMap& operator=(const ShadowMap& lhs) = default;

private:
	// square 512, 1024, 2048, 4096 
	UINT mWidth;
	UINT mHeight;
	
	ID3D11Texture2D* mShadowMap;
	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	D3D11_VIEWPORT mViewport;
	
	ID3D11SamplerState* mSamplerState;

	ID3D11RasterizerState* mRasterizerState;
	ID3D11RasterizerState* mShadowRastState;
};