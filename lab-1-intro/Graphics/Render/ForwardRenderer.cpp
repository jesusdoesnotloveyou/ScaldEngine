#include "ForwardRenderer.h"

ForwardRenderer::ForwardRenderer(IDXGISwapChain* spawChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
	: Renderer(spawChain, device, deviceContext, width, height)
{
}

void ForwardRenderer::SetupShaders()
{
	Renderer::SetupShaders();

	D3D11_INPUT_ELEMENT_DESC inputLayoutDefaultDesc[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0u,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0u,
			0u,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0u,
			DXGI_FORMAT_R32G32_FLOAT,
			0u,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0u,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0u,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u}
	};

	ThrowIfFailed(mVertexShader.Init(mDevice, inputLayoutDefaultDesc, (UINT)std::size(inputLayoutDefaultDesc), L"./Shaders/VertexShader.hlsl"));
	ThrowIfFailed(mPixelShader.Init(mDevice, L"./Shaders/PixelShader.hlsl"));
}