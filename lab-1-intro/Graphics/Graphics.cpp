#include "../ScaldException.h"
#include "ScaldCoreTypes.h"
#include "Graphics.h"
#include <chrono>

#include "../Objects/Geometry/PrimitiveGeometry.h"

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

Graphics::Graphics(HWND hWnd, int width, int height)
	:
	mScreenWidth(width),
	mScreenHeight(height),
	hWnd(hWnd)
{
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = { };
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = mScreenWidth;
	swapDesc.BufferDesc.Height = mScreenHeight;
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

	// Step 03: Get the BackBuffer and create RTV
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackTex;
	// gain access to texture subresource in swap chain (back buffer)
	// check MSDN for more info about GetAddressOf, Get, (&) ReleaseAndGetAddressOf
	ThrowIfFailed(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackTex));
	ThrowIfFailed(pDevice->CreateRenderTargetView(pBackTex.Get(), nullptr, &pRtv));


	//// Describe our Depth/Stencil Buffer
	//D3D11_TEXTURE2D_DESC depthStencilDesc;
	//depthStencilDesc.Width = mScreenWidth;
	//depthStencilDesc.Height = mScreenHeight;
	//depthStencilDesc.MipLevels = 1u;
	//depthStencilDesc.ArraySize = 1u;
	//depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthStencilDesc.SampleDesc.Count = 1u;
	//depthStencilDesc.SampleDesc.Quality = 0u;
	//depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//depthStencilDesc.CPUAccessFlags = 0u;
	//depthStencilDesc.MiscFlags = 0u;

	//ThrowIfFailed(pDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf()));
	//ThrowIfFailed(pDevice->CreateDepthStencilView(this->depthStencilBuffer.Get(), nullptr, pDsv.GetAddressOf()));
	//
	////Create depth stencil state
	//D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	//ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//depthstencildesc.DepthEnable = true;
	//depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	//depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	//ThrowIfFailed(pDevice->CreateDepthStencilState(&depthstencildesc, pDepthStencilState.GetAddressOf()));
}

void Graphics::EndFrame()
{
	// Step 15: At the End of While (!isExitRequested): Present the Result
	pContext->OMSetRenderTargets(0, nullptr, nullptr);
	ThrowIfFailed(pSwapChain->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}

ID3D11DeviceContext* Graphics::GetDeviceContext() const
{
	return pContext.Get();
}

ID3D11Device* Graphics::GetDevice() const
{
	return pDevice.Get();
}

void Graphics::SetupShaders()
{
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

	ThrowIfFailed(mVertexShader.Init(GetDevice(), inputElements, (UINT)std::size(inputElements)));
	ThrowIfFailed(mPixelShader.Init(GetDevice()));
}

void Graphics::Setup()
{	
	SetupShaders();

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	ThrowIfFailed(pDevice->CreateRasterizerState(&rastDesc, &pRasterizerState));
	pContext->RSSetState(pRasterizerState.Get());

	// Camera setup
	mCamera.SetPosition(0.0f, 0.0f, -2.0f);
	mCamera.SetProjectionValues(90.0f, static_cast<float>(mScreenWidth) / mScreenHeight, 0.1f, 1000.0f);
}

void Graphics::DrawScene(std::vector<PrimitiveGeometry*>& gameObjects)
{
	pContext->ClearState();
	pContext->RSSetState(pRasterizerState.Get());

	// Step 10: Setup Rasterizer Stage and Viewport
	D3D11_VIEWPORT viewport = {
		D3D11_VIEWPORT {
			0.0f /*TopLeftX*/,
			0.0f /*TopLeftY*/,
			static_cast<float>(mScreenWidth),
			static_cast<float>(mScreenHeight),
			0.0f /*MinDepth*/,
			1.0f /*MaxDepth*/ },
	};
	pContext->RSSetViewports(1u, &viewport);

	for (auto geometry : gameObjects)
	{
		// Step 08: Setup the IA stage
		pContext->IASetInputLayout(mVertexShader.GetInputLayout());
		pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		if (!geometry->GetConstantBuffer().ApplyChanges(mCamera.GetViewMatrix(), mCamera.GetProjectionMatrix())) return;
		pContext->VSSetConstantBuffers(0u, 1u, geometry->GetConstantBuffer().GetAddressOf());
		pContext->IASetVertexBuffers(0u, 1u, geometry->GetVertexBuffer().GetAddressOf(), geometry->GetVertexBuffer().GetStridePtr(), &geometry->offset);
		pContext->IASetIndexBuffer(geometry->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
		
		// Step 09: Set Vertex and Pixel Shaders
		pContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		pContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		// Step 11: Set BackBuffer for Output merger
		pContext->OMSetRenderTargets(1u, pRtv.GetAddressOf(), nullptr);

		// Step 14: At the End of While (!isExitRequested): Draw the Triangle
		pContext->DrawIndexed(geometry->GetIndexBuffer().GetBufferSize(), 0u, 0);
	}
}

void Graphics::ClearBuffer(float r)
{
	float color[] = { r, 0.1f, 0.1f, 1.0f };
	pContext->ClearRenderTargetView(pRtv.Get(), color);
	//pContext->ClearDepthStencilView(pDsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}