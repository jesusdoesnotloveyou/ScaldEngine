#pragma once

#include "Renderer.h"

constexpr UINT BUFFER_COUNT = 3u;

struct TextureRenderTarget
{
	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
};

class DeferredRenderer final : public Renderer
{
public:
	DeferredRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
	virtual ~DeferredRenderer() noexcept override;

public:
	void BindGeometryPass();
	void BindLightingPass();
	void BindTransparentPass();
	
	void Draw();

private:
	// Graphics context. Graphics object manages these resources.
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;

	// Deferred Renderer specific
	TextureRenderTarget mGBuffer[BUFFER_COUNT];
};