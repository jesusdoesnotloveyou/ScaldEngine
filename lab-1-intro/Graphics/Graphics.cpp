#include "../ScaldException.h"
#include "Graphics.h"
#include <chrono>

#include "Camera.h"
#include "ThirdPersonCamera.h"
#include "../Objects/Geometry/Actor.h"
#include "Light/LightHelper.h"
#include "Light/Light.h"

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
	D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
	depthStencilTextureDesc.Width = mScreenWidth;
	depthStencilTextureDesc.Height = mScreenHeight;
	depthStencilTextureDesc.MipLevels = 1u;
	depthStencilTextureDesc.ArraySize = 1u;
	depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTextureDesc.SampleDesc.Count = 1u;
	depthStencilTextureDesc.SampleDesc.Quality = 0u;
	depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDesc.CPUAccessFlags = 0u;
	depthStencilTextureDesc.MiscFlags = 0u;

	ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilTextureDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));
	ThrowIfFailed(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDsv.GetAddressOf()));

	// Step 11: Set BackBuffer for Output merger
	mDeviceContext->OMSetRenderTargets(1u, mRtv.GetAddressOf(), mDsv.Get());

	//mCamera = new Camera();
	mTPCamera = new ThirdPersonCamera();
}

Graphics::~Graphics()
{
	//if (mCamera) delete mCamera;
	if (mTPCamera) delete mTPCamera;
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

	ThrowIfFailed(mVertexShader.Init(mDevice.Get(), inputElements, (UINT)std::size(inputElements)));
	ThrowIfFailed(mPixelShader.Init(mDevice.Get()));
}

void Graphics::SetupLight()
{
	ThrowIfFailed(CreateStructuredBuffer(mDevice.Get(), mLightBuffer.GetAddressOf(), mLightsParameters));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0u;
	srvDesc.Buffer.NumElements = (UINT)mLightsParameters.size();

	ThrowIfFailed(mDevice->CreateShaderResourceView(mLightBuffer.Get(), &srvDesc, &mLightShaderResourceView));
}

void Graphics::Setup()
{
	//Create depth stencil state
	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(CD3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	ThrowIfFailed(mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf()));

	// Step 10: Setup Rasterizer Stage and Viewport
	D3D11_VIEWPORT currentViewport;
	currentViewport.TopLeftX = 0.0f;
	currentViewport.TopLeftY = 0.0f;
	currentViewport.Width = static_cast<float>(mScreenWidth);
	currentViewport.Height = static_cast<float>(mScreenHeight);
	currentViewport.MinDepth = 0.0f;
	currentViewport.MaxDepth = 1.0f;
	mDeviceContext->RSSetViewports(1u, &currentViewport);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	ThrowIfFailed(mDevice->CreateRasterizerState(&rastDesc, mRasterizerState.GetAddressOf()));

	D3D11_SAMPLER_DESC sampDesc = {};
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ThrowIfFailed(mDevice->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf()));

	// Camera setup
	mTPCamera->SetProjectionValues(90.0f, static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight), 0.1f, 3000.0f);
	mTPCamera->SetOrthographicProjectionValues(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight), 0.1f, 3000.0f);

	ThrowIfFailed(mCBPerFrame.Init(mDevice.Get(), mDeviceContext.Get()));
}

void Graphics::InitSceneObjects(std::vector<SceneGeometry*>& sceneObjects)
{
	SetupShaders();
	if (bIsPointLightEnabled)
	{
		SetupLight();
	}
	
	mTPCamera->SetTarget(sceneObjects[0]);

	for (auto sceneObject : sceneObjects)
	{
		sceneObject->Init(mDevice.Get(), mDeviceContext.Get());
	}
}

void Graphics::AddLightSourceParams(PointLight* lightParams)
{
	mLightsParameters.push_back(*lightParams);
}

void Graphics::UpdateLightParams(std::vector<SceneGeometry*>& lightObjects)
{
	for (int i = 0; i < lightObjects.size(); i++)
	{
		auto light = static_cast<Light*>(lightObjects[i]);
		if (!light) continue;

		mLightsParameters[i] = *light->GetPointLightParams();
	}
}

void Graphics::DrawScene(std::vector<SceneGeometry*>& sceneObjects)
{
	// Step 11: Set BackBuffer for Output merger
	mDeviceContext->OMSetRenderTargets(1u, mRtv.GetAddressOf(), mDsv.Get());

	mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDeviceContext->RSSetState(mRasterizerState.Get());
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);
	mDeviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());
	// for shadows
	//mDeviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());

#pragma region ConstBufferPSPerFrame
	ConstBufferPSPerFrame cbPerFrame;

	// default DirectionalLight params
	cbPerFrame.dirLight.ambient = { 1.0f, 1.0f, 1.0f, 0.9f };
	cbPerFrame.gEyePos = mTPCamera->GetPosition();
	cbPerFrame.numLights = (float)mLightsParameters.size();

	mCBPerFrame.SetData(cbPerFrame);
	mCBPerFrame.ApplyChanges();

	mDeviceContext->PSSetConstantBuffers(1u, 1u, mCBPerFrame.GetAddressOf());
#pragma endregion ConstBufferPSPerFrame

	if (bIsPointLightEnabled)
	{
		ApplyChanges(mDeviceContext.Get(), mLightBuffer.Get(), mLightsParameters);
		mDeviceContext->PSSetShaderResources(1u, 1u, mLightShaderResourceView.GetAddressOf());
	}
	// Step 09: Set Vertex and Pixel Shaders
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	for (auto actor : sceneObjects)
	{
		actor->Draw(mTPCamera->GetViewMatrix() * mTPCamera->GetProjectionMatrix());
	}
}

// Before rendering every frame we should clear render target view and depth stencil view
void Graphics::ClearBuffer(float r)
{
	float color[] = { r, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRtv.Get(), color);
	mDeviceContext->ClearDepthStencilView(mDsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL /*Clearing Both Depth and Stencil*/, 1.0f, 0u);
	//mDeviceContext->ClearState();
}

void Graphics::EndFrame()
{
	// Step 15: At the End of While (!isExitRequested): Present the Result
	mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	ThrowIfFailed(mSwapChain->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}