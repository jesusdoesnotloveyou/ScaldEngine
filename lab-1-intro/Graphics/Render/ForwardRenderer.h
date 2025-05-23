#pragma once

#include "Renderer.h"

class ForwardRenderer final : public Renderer
{
public:
	ForwardRenderer(IDXGISwapChain* spawChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
	virtual ~ForwardRenderer() noexcept override = default;
	
	// Begin of Renderer interface
	virtual void SetupShaders() override;

private:
	// Forward rendering shaders
	VertexShader mVertexShader;
	PixelShader mPixelShader;
};