#include "../ScaldException.h"
#include "ScaldCoreTypes.h"
#include "Graphics.h"
#include <chrono>

#include "../Objects/Geometry/2D/PrimitiveGeometry.h"

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
		&mSwapChain,
		&mDevice,
		nullptr,
		&mDeviceContext
	));

	// Step 03: Get the BackBuffer and create RTV
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackTex;
	// gain access to texture subresource in swap chain (back buffer)
	// check MSDN for more info about GetAddressOf, Get, (&) ReleaseAndGetAddressOf
	ThrowIfFailed(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackTex));
	ThrowIfFailed(mDevice->CreateRenderTargetView(pBackTex.Get(), nullptr, &mRtv));

	// Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mScreenWidth;
	depthStencilDesc.Height = mScreenHeight;
	depthStencilDesc.MipLevels = 1u;
	depthStencilDesc.ArraySize = 1u;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1u;
	depthStencilDesc.SampleDesc.Quality = 0u;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0u;
	depthStencilDesc.MiscFlags = 0u;

	ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));
	ThrowIfFailed(mDevice->CreateDepthStencilView(this->mDepthStencilBuffer.Get(), nullptr, mDsv.GetAddressOf()));
	
	//Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	ThrowIfFailed(mDevice->CreateDepthStencilState(&depthstencildesc, mDepthStencilState.GetAddressOf()));
}

ID3D11DeviceContext* Graphics::GetDeviceContext() const
{
	return mDeviceContext.Get();
}

ID3D11Device* Graphics::GetDevice() const
{
	return mDevice.Get();
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
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;

	ThrowIfFailed(mDevice->CreateRasterizerState(&rastDesc, &mRasterizerState)); // or mRasterizerState.GetAddressOf()
	mDeviceContext->RSSetState(mRasterizerState.Get());

	D3D11_SAMPLER_DESC sampDesc = {};
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ThrowIfFailed(mDevice->CreateSamplerState(&sampDesc, &mSamplerState)); // or mSamplerState.GetAddressOf()

	// Camera setup
	mCamera.SetPosition(0.0f, 50.0f, -80.0f);
	mCamera.SetProjectionValues(90.0f, static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight), 0.1f, 1000.0f);
}

void Graphics::DrawScene(std::vector<PrimitiveGeometry*>& gameObjects)
{
	mDeviceContext->ClearState();
	mDeviceContext->RSSetState(mRasterizerState.Get());

	// Step 10: Setup Rasterizer Stage and Viewport
	D3D11_VIEWPORT currentViewport;
		currentViewport.TopLeftX = 0.0f;
		currentViewport.TopLeftY = 0.0f;
		currentViewport.Width = static_cast<float>(mScreenWidth);
		currentViewport.Height = static_cast<float>(mScreenHeight);
		currentViewport.MinDepth = 0.0f;
		currentViewport.MaxDepth = 1.0f;

	mDeviceContext->RSSetViewports(1u, &currentViewport);

	for (auto geometry : gameObjects)
	{
		// Step 08: Setup the IA stage
		mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
		mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		mDeviceContext->RSSetState(mRasterizerState.Get());
		mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

		// Step 09: Set Vertex and Pixel Shaders
		mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		if (!geometry->GetConstantBuffer().ApplyChanges(mCamera.GetViewMatrix(), mCamera.GetProjectionMatrix())) continue;
		mDeviceContext->IASetVertexBuffers(0u, 1u, geometry->GetVertexBuffer().GetAddressOf(), geometry->GetVertexBuffer().GetStridePtr(), &geometry->offset);
		mDeviceContext->IASetIndexBuffer(geometry->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
		mDeviceContext->VSSetConstantBuffers(0u, 1u, geometry->GetConstantBuffer().GetAddressOf());

		// Step 11: Set BackBuffer for Output merger
		mDeviceContext->OMSetRenderTargets(1u, mRtv.GetAddressOf(), mDsv.Get());

		// Step 14: At the End of While (!isExitRequested): Draw the Triangle
		mDeviceContext->DrawIndexed(geometry->GetIndexBuffer().GetBufferSize(), 0u, 0);
	}
}

// Before rendering every frame we should clear render target view and depth stencil view
void Graphics::ClearBuffer(float r)
{
	float color[] = { r, 0.1f, 0.1f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRtv.Get(), color);
	mDeviceContext->ClearDepthStencilView(mDsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL /*Clearing Both Depth and Stencil*/, 1.0f, 0u);
}

void Graphics::EndFrame()
{
	// Step 15: At the End of While (!isExitRequested): Present the Result
	mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	ThrowIfFailed(mSwapChain->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}