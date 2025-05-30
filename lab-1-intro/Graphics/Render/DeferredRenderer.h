#pragma once

#include "Renderer.h"

constexpr UINT BUFFER_COUNT = 3u;

class Mesh;

struct TextureRenderTarget
{
	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
};

class DeferredRenderer final : public Renderer
{
public:
	DeferredRenderer(IDXGISwapChain* spawChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
	virtual ~DeferredRenderer() noexcept override;

	// Begin of Renderer interface
	virtual void SetupShaders() override;
	virtual void CreateRasterizerState() override;
	// End of Renderer interface

public:
	void BindGeometryPass();
	void BindLightingPass();
	void BindTransparentPass();
	
	void DrawScreenQuad();
	// deferred additional task
	void DrawGBuffer();
	FORCEINLINE void ChangeGBufferLayer(int layer) { GBufferLayer = layer; }

	void BindWithinFrustum();
	void BindIntersectsFarPlane();
	void BindOutsideFrustum();

private:
	// Deferred Renderer specific
	VertexShader mOpaqueVertexShader;
	PixelShader mOpaquePixelShader;
	VertexShader mLightingVertexShader;
	PixelShader mLightingPixelShader;

	// deferred additional task
	VertexShader mGBufferVS;
	PixelShader mGBufferPS;

	TextureRenderTarget mGBuffer[BUFFER_COUNT];

	Mesh* screenQuad = nullptr;

	// deferred additional task
	Mesh* GBufferTexture = nullptr;
	int GBufferLayer = 0;

	D3D11_VIEWPORT mGBufferViewport = {};
};