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

public:
	void UpdateShadowCascadeSplits(float cameraNearZ, float cameraFarZ);
	void SetCascadeLevelValue(UINT index, float value);
	FORCEINLINE float GetCascadeLevel(UINT index)const { return shadowCascadeLevels[index]; }

private:
	// shadow map resolution
	UINT mWidth = 2048;
	UINT mHeight = 2048;

	ID3D11ShaderResourceView* mDepthMapSRV;
	ID3D11DepthStencilView* mDepthMapDSV;
	D3D11_VIEWPORT mViewport;

private:
	float cascadeSplitLambda = 0.95f; // idk (probably magic number from Nvidia algorithm
	float shadowCascadeLevels[CASCADE_NUMBER] = { 0.0f, 0.0f, 0.0f, 0.0f };
};