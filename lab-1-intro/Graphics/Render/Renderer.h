#pragma once

#include <d3d11.h>

class Renderer
{
public:
	Renderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
	virtual ~Renderer() noexcept = default;


protected:
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	D3D11_VIEWPORT mViewport = {};
};