#include "Graphics.h"
#include "ScaldException.h"
#include <chrono>

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

Graphics::Graphics(HWND hWnd, int width, int height)
	:
	screenWidth(width),
	screenHeight(height),
	hWnd(hWnd)
{
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = { };
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = screenWidth;
	swapDesc.BufferDesc.Height = screenHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	));
}

void Graphics::EndFrame()
{
	// empty for now, but supposed to clear state and buffer
	//ClearBuffer();
	Present();
}

void Graphics::DrawRectangle()
{
	// Step 04: Compile the Shaders
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexBC = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorVertexCode = nullptr;
	ThrowIfFailed(D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0u,
		&pVertexBC,
		&pErrorVertexCode));

	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

	Microsoft::WRL::ComPtr<ID3DBlob> pPixelBC;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorPixelCode;
	ThrowIfFailed(D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0" /*pixel shader*/,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0u,
		&pPixelBC,
		&pErrorPixelCode));

	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	ThrowIfFailed(pDevice->CreateVertexShader(
		pVertexBC->GetBufferPointer(),
		pVertexBC->GetBufferSize(),
		nullptr, &pVertexShader));

	ThrowIfFailed(pDevice->CreatePixelShader(
		pPixelBC->GetBufferPointer(),
		pPixelBC->GetBufferSize(),
		nullptr, &pPixelShader));

	// Step 05: Create Input Layout for IA Stage
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0u,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0u,
			0u,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0u,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0u,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u}
	};

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;
	ThrowIfFailed(pDevice->CreateInputLayout(
		inputElements,
		2u,
		pVertexBC->GetBufferPointer(),
		pVertexBC->GetBufferSize(),
		&pLayout));

	// Step 06: Create Set of Points
	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	// Step 07: Create Vertex and Index Buffers
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0u;
	vertexBufDesc.MiscFlags = 0u;
	vertexBufDesc.StructureByteStride = 0u;
	vertexBufDesc.ByteWidth = sizeof(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVb;
	ThrowIfFailed(pDevice->CreateBuffer(&vertexBufDesc, &vertexData, &pVb));

	int indeces[] = { 0, 1, 2,  1, 0, 3 };
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0u;
	indexBufDesc.MiscFlags = 0u;
	indexBufDesc.StructureByteStride = 0u;
	indexBufDesc.ByteWidth = sizeof(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0u;
	indexData.SysMemSlicePitch = 0u;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pIb;
	ThrowIfFailed(pDevice->CreateBuffer(&indexBufDesc, &indexData, &pIb));

	UINT strides[] = { 32 };
	UINT offsets[] = { 0 };

	// Step 10: Setup Rasterizer Stage and Viewport
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
	ThrowIfFailed(pDevice->CreateRasterizerState(&rastDesc, &rastState));

	pContext->RSSetState(rastState.Get());

	/*std::chrono::time_point<std::chrono::steady_clock> PrevTime = std::chrono::steady_clock::now();
	float totalTime = 0;
	unsigned int frameCount = 0;*/

	pContext->ClearState();
	pContext->RSSetState(rastState.Get());
	
	// Step 10: Setup Rasterizer Stage and Viewport
	D3D11_VIEWPORT viewport = {
		D3D11_VIEWPORT {
			0.0f /*TopLeftX*/,
			0.0f /*TopLeftY*/,
			static_cast<float>(screenWidth),
			static_cast<float>(screenHeight),
			0.0f /*MinDepth*/,
			1.0f /*MaxDepth*/ },
	};

	// Step 03: Get the BackBuffer and create RTV
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backTex;
	// gain access to texture subresource in swap chain (back buffer)
	// check MSDN for more info about GetAddressOf, Get, (&) ReleaseAndGetAddressOf
	ThrowIfFailed(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backTex));
	ThrowIfFailed(pDevice->CreateRenderTargetView(backTex.Get(), nullptr, &pRtv));
	// Step 11: Set BackBuffer for Output merger
	pContext->OMSetRenderTargets(1u, pRtv.GetAddressOf(), nullptr);

	float color[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	// Step 13: At the End of While (!isExitRequested): Clear BackBuffer
	pContext->ClearRenderTargetView(pRtv.Get(), color);

	pContext->RSSetViewports(1u, &viewport);

	// Step 08: Setup the IA stage
	pContext->IASetInputLayout(pLayout.Get());
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetIndexBuffer(pIb.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetVertexBuffers(0, 1, pVb.GetAddressOf(), strides, offsets);
	// Step 09: Set Vertex and Pixel Shaders
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	// Step 14: At the End of While (!isExitRequested): Draw the Triangle
	pContext->DrawIndexed(6u, 0u, 0);

	pContext->OMSetRenderTargets(0, nullptr, nullptr);
}

void Graphics::ClearBuffer()
{
	float color[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	pContext->ClearRenderTargetView(pRtv.Get(), color);
}

void Graphics::SetupViewports(UINT NumViewports, D3D11_VIEWPORT* pViewports)
{
	auto viewPortOffsetX = 0.f;
	for (UINT i = 0; i < NumViewports; i++)
	{
		pViewports[NumViewports].Width = static_cast<float>(screenWidth / NumViewports);
		pViewports[NumViewports].Height = static_cast<float>(screenHeight / NumViewports);

		pViewports[NumViewports].TopLeftX = viewPortOffsetX;
		pViewports[NumViewports].TopLeftY = 100;
		pViewports[NumViewports].MinDepth = 0;
		pViewports[NumViewports].MaxDepth = 1.0f;
		viewPortOffsetX += pViewports[NumViewports].Width;
	}
	pContext->RSSetViewports(NumViewports, pViewports);
}

void Graphics::Present()
{
	// Step 15: At the End of While (!isExitRequested): Present the Result
	ThrowIfFailed(pSwapChain->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}