#pragma once

#include <d3d11.h>

constexpr UINT BUFFER_COUNT = 4u;

struct TextureRenderTarget
{
	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
};

class DeferredRenderer
{
public:
	DeferredRenderer(ID3D11Device* device, UINT width, UINT height);
	~DeferredRenderer() noexcept;

public:
	void BindGeometryPass();
	void BindLightingPass();
	void BindTransparentPass();
	
	void Draw();

private:
	TextureRenderTarget mGBuffer[BUFFER_COUNT];

	ID3D11DepthStencilView* mDepthStencilView = nullptr;
};