#include "../ScaldException.h"
#include "Graphics.h"
#include <chrono>
#include <algorithm>

#include "Camera/ThirdPersonCamera.h"
#include "../Objects/Geometry/Actor.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"
#include "Light/SpotLight.h"
#include "Shadows/CascadeShadowMap.h"

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
	ThrowIfFailed(mDevice->CreateRenderTargetView(pBackTex.Get(), nullptr, &mRTV));

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
	ThrowIfFailed(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDSV.GetAddressOf()));

	mCascadeShadowMap = new CascadeShadowMap(mDevice.Get(), 2048u, 2048u);
	mTPCamera = new ThirdPersonCamera();
}

Graphics::~Graphics()
{
	if (mTPCamera) delete mTPCamera;
	if (mCascadeShadowMap) delete mCascadeShadowMap;
	if (shadowCascadeLevels) delete[] shadowCascadeLevels;
}

void Graphics::Setup()
{
	CreateDepthStencilState();
	CreateRasterizerState();
	CreateSamplerState();

	// Camera setup
	mTPCamera->SetPerspectiveProjectionValues(mFovDegrees, static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight), mCameraNearZ, mCameraFarZ);
	mTPCamera->SetOrthographicProjectionValues(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight), mCameraNearZ, mCameraFarZ);

	// for cascade shadows
	UpdateShadowCascadeSplits();

	// constant buffers setup
	ThrowIfFailed(mCBVSPerFrame.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCBPSPerFrame.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCB_CSM.Init(mDevice.Get(), mDeviceContext.Get()));
}

void Graphics::AddToRenderPool(SceneGeometry* sceneObject)
{
	mRenderObjects.push_back(sceneObject);

	const auto lightObject = dynamic_cast<Light*>(sceneObject);
	if (!lightObject) return;

	if (lightObject->GetLightType() == ELightType::Point)
	{
		const auto pointLight = static_cast<PointLight*>(lightObject);
		mPointLights.push_back(pointLight);
		AddPointLightSourceParams(pointLight->GetParams());
	}
	if (lightObject->GetLightType() == ELightType::Directional)
	{
		const auto dirLight = static_cast<DirectionalLight*>(lightObject);
		mDirectionalLights.push_back(dirLight);
		AddDirectionalLightSourceParams(dirLight->GetParams());
	}
	if (lightObject->GetLightType() == ELightType::Spot)
	{
		const auto spotLight = static_cast<SpotLight*>(lightObject);
		mSpotLights.push_back(spotLight);
		AddSpotLightSourceParams(spotLight->GetParams());
	}
}

void Graphics::InitSceneObjects()
{
	if (mRenderObjects.empty()) return; // assert or smth

	SetupShaders();
	mTPCamera->SetTarget(mRenderObjects[0]);

	if (bIsPointLightEnabled)
	{
		InitPointLight();
	}

	if (bIsDirectionalLightEnabled)
	{
		InitDirectionalLight();
	}

	for (auto sceneObject : mRenderObjects)
	{
		sceneObject->Init(mDevice.Get(), mDeviceContext.Get());
	}
}

void Graphics::AddPointLightSourceParams(PointLightParams* lightParams)
{
	mPointLightsParameters.push_back(*lightParams);
}

void Graphics::UpdatePointLightParams()
{
	if (mPointLights.empty()) return;
	for (int i = 0; i < mPointLights.size(); i++)
	{
		mPointLightsParameters[i] = *mPointLights[i]->GetParams();
	}
}

void Graphics::AddDirectionalLightSourceParams(DirectionalLightParams* lightParams)
{
	mDirectionalLightParameters.push_back(*lightParams);
}

void Graphics::UpdateDirectionalLightParams()
{
	if (mDirectionalLights.empty()) return;
	for (int i = 0; i < mDirectionalLights.size(); i++)
	{
		mDirectionalLightParameters[i] = *mDirectionalLights[i]->GetParams();
	}
}

void Graphics::AddSpotLightSourceParams(SpotLightParams* lightParams)
{

}

void Graphics::UpdateSpotLightParams()
{

}

// Before rendering every frame we should clear render target view and depth stencil view
void Graphics::ClearBuffer(float r)
{
	float color[] = { r, 0.0f, 0.0f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRTV.Get(), color);
	mDeviceContext->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL /*Clearing Both Depth and Stencil*/, 1.0f, 0u);
}

void Graphics::DrawScene()
{
	RenderDepthOnlyPass();

	mDeviceContext->ClearState();

	RenderColorPass();
}

void Graphics::RenderDepthOnlyPass()
{
	// imporatnt to fix d3d warning
	ID3D11ShaderResourceView* nullSrv[2] = { nullptr, nullptr };
	mDeviceContext->PSSetShaderResources(0u, 2u, nullSrv);

	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mCascadeShadowMap->BindDsvAndSetNullRenderTarget(mDeviceContext.Get());
	mDeviceContext->IASetInputLayout(mShadowVertexShader.GetInputLayout());
	mDeviceContext->VSSetShader(mShadowVertexShader.Get(), nullptr, 0u);
	// for CSM
	mDeviceContext->GSSetShader(mCSMGeometryShader.Get(), nullptr, 0u);

	mDeviceContext->PSSetShader(nullptr, nullptr, 0u);

	// usually we have only one dir light source, but i decided to leave it like generic case could be with multiple
	for (auto dirLight : mDirectionalLights)
	{
		CascadeData cbGS_CSM = {};
		GetLightSpaceMatrices();
		
		for (UINT i = 0; i < CASCADE_NUMBER; i++)
		{
			cbGS_CSM.ViewProj[i] = XMMatrixTranspose(lightSpaceMatrices[i]);
			cbGS_CSM.distances[i] = 0.0f; // not used on GPU, so filled with zero
		}

		mCB_CSM.SetData(cbGS_CSM);
		mCB_CSM.ApplyChanges();
		mDeviceContext->GSSetConstantBuffers(0u, 1u, mCB_CSM.GetAddressOf());

		for (auto actor : mRenderObjects)
		{
			if (actor == mDirectionalLights[0]) continue;
			actor->Draw(XMMatrixIdentity(), XMMatrixIdentity());
		}
	}
}

void Graphics::RenderColorPass()
{
	// Have to set primitive topology again, since we call ClearState between passes
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());

	// Step 11: Set BackBuffer for Output merger
	mDeviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), mDSV.Get());

	// Step 09: Set Vertex Shaders
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0u);

	// @todo: temporary solution, need a redesign
#pragma region ConstBufferVSPerFrame
	ConstBufferVSPerFrame cbVSPerFrame;
	cbVSPerFrame.gLightViewProjection = XMMatrixTranspose(mDirectionalLights[0]->GetViewMatrix() * mDirectionalLights[0]->GetOrthographicProjectionMatrix());
	mCBVSPerFrame.SetData(cbVSPerFrame);
	mCBVSPerFrame.ApplyChanges();

	mDeviceContext->VSSetConstantBuffers(1u, 1u, mCBVSPerFrame.GetAddressOf());
#pragma endregion ConstBufferVSPerFrame

	mDeviceContext->RSSetViewports(1u, &currentViewport);
	mDeviceContext->RSSetState(mRasterizerState.Get());

	mDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0u);

	// Step 09: Set Pixel Shaders
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0u);
	mDeviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());
	mDeviceContext->PSSetSamplers(1u, 1u, mShadowSamplerState.GetAddressOf());

#pragma region ConstBufferPSPerFrame
	ConstBufferPSPerFrame cbPSPerFrame;
	cbPSPerFrame.gEyePos = mTPCamera->GetPosition();
	// should be placed in other const buffer (cause there is no need to update these members every frame
	cbPSPerFrame.numPointLights = (float)mPointLightsParameters.size();
	cbPSPerFrame.numDirectionalLights = (float)mDirectionalLightParameters.size();

	mCBPSPerFrame.SetData(cbPSPerFrame);
	mCBPSPerFrame.ApplyChanges();
	mDeviceContext->PSSetConstantBuffers(0u, 1u, mCBPSPerFrame.GetAddressOf());
#pragma endregion ConstBufferPSPerFrame

	CascadeData cbPS_CSM;
	for (UINT i = 0; i < CASCADE_NUMBER; ++i)
	{
		cbPS_CSM.ViewProj[i] = XMMatrixTranspose(lightSpaceMatrices[i]);;
		cbPS_CSM.distances[i] = shadowCascadeLevels[i];
	}
	mCB_CSM.SetData(cbPS_CSM);
	mCB_CSM.ApplyChanges();
	// send cascades data to GPU
	mDeviceContext->PSSetConstantBuffers(1u, 1u, mCB_CSM.GetAddressOf());
	
	// Bind shadow maps from depth pass to pixel shader
	mDeviceContext->PSSetShaderResources(1u, 1u, mCascadeShadowMap->GetAddressOf());

	if (bIsPointLightEnabled)
	{
		ApplyChanges(mDeviceContext.Get(), mPointLightBuffer.Get(), mPointLightsParameters);
		mDeviceContext->PSSetShaderResources(2u, 1u, mPointLightShaderResourceView.GetAddressOf());
	}
	if (bIsDirectionalLightEnabled)
	{
		ApplyChanges(mDeviceContext.Get(), mDirectionalLightBuffer.Get(), mDirectionalLightParameters);
		mDeviceContext->PSSetShaderResources(3u, 1u, mDirectionalLightShaderResourceView.GetAddressOf());
	}

	for (auto actor : mRenderObjects)
	{
		actor->Draw(mTPCamera->GetViewMatrix(), mTPCamera->GetPerspectiveProjectionMatrix());
	}
}

void Graphics::EndFrame()
{
	// Step 15: At the End of While (!isExitRequested): Present the Result
	mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	ThrowIfFailed(mSwapChain->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}

void Graphics::Update(const ScaldTimer& st)
{
	mTPCamera->Update(st);
	UpdatePointLightParams();
	UpdateDirectionalLightParams();
	//UpdateSpotLightParams();
}

void Graphics::CreateDepthStencilState()
{
	//Create depth stencil state
	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(CD3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	ThrowIfFailed(mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf()));
}

void Graphics::CreateRasterizerState()
{
	//Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;

	// Step 10: Setup Rasterizer Stage and Viewport
	currentViewport.TopLeftX = 0.0f;
	currentViewport.TopLeftY = 0.0f;
	currentViewport.Width = static_cast<float>(mScreenWidth);
	currentViewport.Height = static_cast<float>(mScreenHeight);
	currentViewport.MinDepth = 0.0f;
	currentViewport.MaxDepth = 1.0f;

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthBias;
	rastDesc.DepthBiasClamp;
	rastDesc.SlopeScaledDepthBias;
	ThrowIfFailed(mDevice->CreateRasterizerState(&rastDesc, mRasterizerState.GetAddressOf()));
}

void Graphics::CreateSamplerState()
{
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

	D3D11_SAMPLER_DESC shadowSampDesc = {};
	ZeroMemory(&shadowSampDesc, sizeof(shadowSampDesc));
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSampDesc.MipLODBias = 0.0f;
	shadowSampDesc.MaxAnisotropy = 1u;
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	shadowSampDesc.MinLOD = 0.0f;
	shadowSampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ThrowIfFailed(mDevice->CreateSamplerState(&shadowSampDesc, mShadowSamplerState.GetAddressOf()));
}

void Graphics::SetupShaders()
{
	// Step 05: Create Input Layout for IA Stage
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

	D3D11_INPUT_ELEMENT_DESC inputLayoutShadowDesc[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0u,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0u,
			0u,
			D3D11_INPUT_PER_VERTEX_DATA,
			0u}
	};

	ThrowIfFailed(mShadowVertexShader.Init(mDevice.Get(), inputLayoutShadowDesc, (UINT)std::size(inputLayoutShadowDesc), L"./Shaders/ShadowVertexShader.hlsl"));
	ThrowIfFailed(mCSMGeometryShader.Init(mDevice.Get(), L"./Shaders/CSMGeometryShader.hlsl"));

	ThrowIfFailed(mVertexShader.Init(mDevice.Get(), inputLayoutDefaultDesc, (UINT)std::size(inputLayoutDefaultDesc), L"./Shaders/VertexShader.hlsl"));
	ThrowIfFailed(mPixelShader.Init(mDevice.Get(), L"./Shaders/FragmentShader.hlsl"));
}

void Graphics::InitPointLight()
{
	ThrowIfFailed(CreateStructuredBuffer(mDevice.Get(), mPointLightBuffer.GetAddressOf(), mPointLightsParameters));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0u;
	srvDesc.Buffer.NumElements = (UINT)mPointLightsParameters.size();

	ThrowIfFailed(mDevice->CreateShaderResourceView(mPointLightBuffer.Get(), &srvDesc, &mPointLightShaderResourceView));
}

void Graphics::InitDirectionalLight()
{
	ThrowIfFailed(CreateStructuredBuffer(mDevice.Get(), mDirectionalLightBuffer.GetAddressOf(), mDirectionalLightParameters));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0u;
	srvDesc.Buffer.NumElements = (UINT)mDirectionalLightParameters.size();

	ThrowIfFailed(mDevice->CreateShaderResourceView(mDirectionalLightBuffer.Get(), &srvDesc, &mDirectionalLightShaderResourceView));
}

std::vector<XMVECTOR> Graphics::GetFrustumCornersWorldSpace(const XMMATRIX& viewProjection)
{
	XMVECTOR det;
	const auto inv = XMMatrixInverse(&det, viewProjection);

	std::vector<XMVECTOR> frustumCorners;
	frustumCorners.reserve(8);
	
	for (UINT x = 0; x < 2; ++x)
	{
		for (UINT y = 0; y < 2; ++y)
		{
			for (UINT z = 0; z < 2; ++z)
			{
				// translate NDC coords to world space
				const XMVECTOR pt = XMVector4Transform(std::move(XMVectorSet(
					2.0f * x - 1.0f,
					2.0f * y - 1.0f,
					(float)z, 
					1.0f)), inv);
				frustumCorners.push_back(pt / XMVectorGetW(pt));
			}
		}
	}
	return frustumCorners;
}

std::vector<XMVECTOR> Graphics::GetFrustumCornersWorldSpace(const XMMATRIX& view, const XMMATRIX& projection)
{
	return GetFrustumCornersWorldSpace(view * projection);
}

void Graphics::GetLightSpaceMatrices()
{
	for (UINT i = 0; i < CASCADE_NUMBER; ++i)
	{
		if (i == 0)
		{
			lightSpaceMatrices.push_back(GetLightSpaceMatrix(mCameraNearZ, shadowCascadeLevels[i]));
		}
		else if (i < CASCADE_NUMBER - 1)
		{
			lightSpaceMatrices.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
		}
		else
		{
			lightSpaceMatrices.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
		}
	}
}

XMMATRIX Graphics::GetLightSpaceMatrix(const float nearPlane, const float farPlane)
{
	const auto cameraProjectionMatrix = XMMatrixPerspectiveFovLH(mTPCamera->GetFovRad(), static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight), nearPlane, farPlane);
	const auto frustumCorners = GetFrustumCornersWorldSpace(mTPCamera->GetViewMatrix(), cameraProjectionMatrix);

	XMVECTOR center = XMVectorZero();
	for (const auto& v : frustumCorners)
	{
		center += v;
	}

	center /= (float)frustumCorners.size();
	const XMFLOAT3 lightDir = mDirectionalLights[0]->GetDirection();
	const auto lightView = XMMatrixLookAtLH(center, center + XMVectorSet(lightDir.x, lightDir.y, lightDir.z, 1.0f), ScaldMath::UpVector);

	// Measuring cascade
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const auto& v : frustumCorners)
	{
		const auto trf = XMVector4Transform(v, lightView);
		minX = std::min(minX, XMVectorGetX(trf));
		maxX = std::max(maxX, XMVectorGetX(trf));
		minY = std::min(minY, XMVectorGetY(trf));
		maxY = std::max(maxY, XMVectorGetY(trf));
		minZ = std::min(minZ, XMVectorGetZ(trf));
		maxZ = std::max(maxZ, XMVectorGetZ(trf));
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 10.0f;
	minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
	maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;

	const auto lightProjection = XMMatrixOrthographicOffCenterLH(minX, maxX, minY, maxY, minZ, maxZ);
	return lightView * lightProjection;
}

void Graphics::UpdateShadowCascadeSplits()
{
	const float minZ = mCameraNearZ;
	const float maxZ = mCameraFarZ;

	const float range = maxZ - minZ;
	const float ratio = maxZ / minZ;

	for (int i = 0; i < CASCADE_NUMBER; i++)
	{
		float p = (i + 1) / (float)(CASCADE_NUMBER);
		float log = (float)(minZ * pow(ratio, p));
		float uniform = minZ + range * p;
		float d = cascadeSplitLambda * (log - uniform) + uniform;
		shadowCascadeLevels[i] = ((d - mCameraNearZ) / range) * maxZ;
	}
}