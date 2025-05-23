#pragma once

#include "../../ScaldException.h"
#include "../Shaders.h"
#include "../ConstantBuffer.h"

class Renderer
{
public:
	Renderer(IDXGISwapChain* spawChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, int width, int height);
	virtual ~Renderer() noexcept = default;

	virtual void SetupShaders();

	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateSamplerState();

	void ClearBuffer(float r);
	void BindDepthOnlyPass();

protected:
	// Graphics context. Graphics object manages these resources.
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;

private:
	int mScreenHeight;
	int mScreenWidth;

protected:
	// Render Target
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRTV;
	// Depth Stencil
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDSV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
	// Rast
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
	// Sampler
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mShadowSamplerState;
	D3D11_VIEWPORT mViewport = {};

protected:
	VertexShader mShadowVertexShader;
	GeometryShader mCSMGeometryShader;
};