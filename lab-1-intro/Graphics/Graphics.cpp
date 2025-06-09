#include "../ScaldException.h"
#include "Graphics.h"
#include <chrono>
#include <algorithm>

#include "Render/DeferredRenderer.h"
#include "Particles/FireParticleSystem.h"
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
	hWnd(hWnd),
	mScreenWidth(width),
	mScreenHeight(height)
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
	mTPCamera = new ThirdPersonCamera();

	// RTV and BackBuffer are created down here in Renderer
	pRenderer = std::make_unique<DeferredRenderer>(mSwapChain.Get(), mDevice.Get(), mDeviceContext.Get(), width, height);

	pFireParticleSystem = std::make_unique<FireParticleSystem>(mDevice.Get(), mDeviceContext.Get(), 4096, XMVectorSet(15.0f, 5.0f, 60.0f, 1.0f), mTPCamera);

	// to renderer probably
	mCascadeShadowMap = new CascadeShadowMap(mDevice.Get(), 2048u, 2048u);
}

Graphics::~Graphics()
{
	if (mTPCamera) delete mTPCamera;
	if (mCascadeShadowMap) delete mCascadeShadowMap;
	if (mDirectionalLight) delete mDirectionalLight;
}

void Graphics::Setup()
{
	SetupShaders();

	CreateDepthStencilState();
	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	// Particles
	pFireParticleSystem->InitializeSystem();

	// Camera setup
	mTPCamera->SetPerspectiveProjectionValues(mFovDegrees, static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight), mCameraNearZ, mCameraFarZ);
	mTPCamera->SetOrthographicProjectionValues(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight), mCameraNearZ, mCameraFarZ);

	// for cascade shadows
	mCascadeShadowMap->UpdateShadowCascadeSplits(mCameraNearZ, mCameraFarZ);

	// constant buffers setup for deferred rendering
	ThrowIfFailed(mCB_LightVolume.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCB_PerFrame.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCB_CSM.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCB_Light.Init(mDevice.Get(), mDeviceContext.Get()));
}

void Graphics::AddToRenderPool(SceneGeometry* sceneObject)
{
	mRenderObjects.push_back(sceneObject);

	const auto lightObject = dynamic_cast<Light*>(sceneObject);
	if (!lightObject) return;

	if (bIsDeferredRenderingTechniqueApplied)
	{
		mLights.push_back(lightObject);
		if (lightObject->GetLightType() == ELightType::Directional)
		{
			mDirectionalLight = lightObject;
		}
	}
}

void Graphics::InitSceneObjects()
{
	if (mRenderObjects.empty()) return; // assert or smth

	mTPCamera->SetTarget(mRenderObjects[0]);

	for (auto sceneObject : mRenderObjects)
	{
		sceneObject->Init(mDevice.Get(), mDeviceContext.Get());
	}
}

void Graphics::AddPointLightSourceParams(PointLightParams* lightParams)
{
	mPointLightsParameters.push_back(*lightParams);
}

void Graphics::UpdatePointLightsParams()
{
	if (mPointLights.empty()) return;
	for (int i = 0; i < mPointLights.size(); i++)
	{
		//mPointLightsParameters[i] = *mPointLights[i]->GetParams();
	}
}

void Graphics::AddSpotLightSourceParams(SpotLightParams* lightParams)
{
	mSpotLightsParameters.push_back(*lightParams);
}

void Graphics::UpdateSpotLightsParams()
{
	if (mSpotLights.empty()) return;
	for (int i = 0; i < mSpotLights.size(); i++)
	{
		//mSpotLightsParameters[i] = *mSpotLights[i]->GetParams();
	}
}

// Before rendering every frame we should clear render target view and depth stencil view
void Graphics::ClearBuffer(float r)
{
	pRenderer->ClearBuffer(r);
}

// Forward rendering
//void Graphics::DrawScene()
//{
//	// @todo: Render->Draw(); // deferred|forward|forward+
//	RenderDepthOnlyPass();
//	mDeviceContext->ClearState();
//	RenderColorPass();
//}

// Deferred rendering
void Graphics::DrawScene(const ScaldTimer& st)
{
	ID3D11ShaderResourceView* nullSrv[3] = { nullptr, nullptr, nullptr };
	mDeviceContext->PSSetShaderResources(0u, 3u, nullSrv);

#pragma region ShadowMapping
	mCascadeShadowMap->BindDsvAndSetNullRenderTarget(mDeviceContext.Get());
	pRenderer->BindDepthOnlyPass();
	RenderDepthOnlyPass();
#pragma endregion ShadowMapping

	mDeviceContext->ClearState();

	pRenderer->BindGeometryPass();
	for (auto actor : mRenderObjects)
	{
		actor->Draw(mTPCamera->GetViewMatrix(), mTPCamera->GetPerspectiveProjectionMatrix());
	}

	mDeviceContext->ClearState();

	pRenderer->BindLightingPass();
	BindLightingPassResources();
	RenderLighting();
	mDeviceContext->ClearState();

	/*pRenderer->BindTransparentPass();
	mDeviceContext->ClearState();*/

	RenderParticles(st.DeltaTime());
	mDeviceContext->ClearState();
}

// For both Forward and Deferred
void Graphics::RenderDepthOnlyPass()
{
	std::vector<XMMATRIX> lightSpaceMatrices;
	GetLightSpaceMatrices(lightSpaceMatrices);
		
	for (UINT i = 0; i < CASCADE_NUMBER; i++)
	{
		mCSMData.ViewProj[i] = XMMatrixTranspose(lightSpaceMatrices[i]);
		mCSMData.distances[i] = mCascadeShadowMap->GetCascadeLevel(i); // not used on GPU in Geometry shader, but still filled
	}

	mCB_CSM.SetAndApplyData(mCSMData);
	mDeviceContext->GSSetConstantBuffers(0u, 1u, mCB_CSM.GetAddressOf());

	for (auto actor : mRenderObjects)
	{
		if (actor == mDirectionalLight) continue;
		actor->Draw(XMMatrixIdentity(), XMMatrixIdentity());
	}
}

void Graphics::BindLightingPassResources()
{
	mCB_CSM.SetAndApplyData(mCSMData);
	mDeviceContext->PSSetConstantBuffers(0u, 1u, mCB_CSM.GetAddressOf());

	mPerFrameData.gEyePos = mTPCamera->GetPosition();
	mPerFrameData.gView = XMMatrixTranspose(mTPCamera->GetViewMatrix());
	mCB_PerFrame.SetAndApplyData(mPerFrameData);
	mDeviceContext->PSSetConstantBuffers(1u, 1u, mCB_PerFrame.GetAddressOf());

	mDeviceContext->PSSetShaderResources(3u, 1u, mCascadeShadowMap->GetAddressOf());
}

// Forward Rendering
void Graphics::RenderColorPass()
{
	// Have to set primitive topology again, since we call ClearState between passes
	mDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mDeviceContext->IASetInputLayout(mVertexShader.GetInputLayout());

	// Step 11: Set BackBuffer for Output merger
	//mDeviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), mDSV.Get());

	// Step 09: Set Vertex Shaders
	mDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0u);

	// @todo: temporary solution, need a redesign
#pragma region ConstBufferVSPerFrame
	ConstBufferVSPerFrame cbVSPerFrame;
	cbVSPerFrame.gLightViewProjection = XMMatrixTranspose(mDirectionalLight->GetViewMatrix() * mDirectionalLight->GetOrthographicProjectionMatrix());
	//mCBVSPerFrame.SetData(cbVSPerFrame);
	//mCBVSPerFrame.ApplyChanges();

	//mDeviceContext->VSSetConstantBuffers(1u, 1u, mCBVSPerFrame.GetAddressOf());
#pragma endregion ConstBufferVSPerFrame

	//mDeviceContext->RSSetViewports(1u, &currentViewport);
	//mDeviceContext->RSSetState(mRasterizerStateCullBack.Get());

	//mDeviceContext->OMSetDepthStencilState(mDepthStencilStateLessThan.Get(), 0u);

	// Step 09: Set Pixel Shaders
	mDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0u);
	//mDeviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());
	//mDeviceContext->PSSetSamplers(1u, 1u, mShadowSamplerState.GetAddressOf());

#pragma region ConstantBufferPerFrame
	ConstantBufferPerFrame cbPSPerFrame;
	cbPSPerFrame.gEyePos = mTPCamera->GetPosition();
	// should be placed in other const buffer (cause there is no need to update these members every frame
	/*cbPSPerFrame.numPointLights = (float)mPointLightsParameters.size();
	cbPSPerFrame.numDirectionalLights = (float)mDirectionalLightParameters.size();*/

	mCB_PerFrame.SetAndApplyData(cbPSPerFrame);
	mDeviceContext->PSSetConstantBuffers(0u, 1u, mCB_PerFrame.GetAddressOf());
#pragma endregion ConstantBufferPerFrame

	// send to GPU cascades data that filled on the depth only pass
	mDeviceContext->PSSetConstantBuffers(1u, 1u, mCB_CSM.GetAddressOf());
	
	// Bind shadow maps from depth pass to pixel shader
	mDeviceContext->PSSetShaderResources(1u, 1u, mCascadeShadowMap->GetAddressOf());

	if (bIsPointLightEnabled)
	{
		ApplyChanges(mDeviceContext.Get(), mPointLightBuffer.Get(), mPointLightsParameters);
		mDeviceContext->PSSetShaderResources(2u, 1u, mPointLightSRV.GetAddressOf());
	}
	if (bIsDirectionalLightEnabled)
	{
		/*ApplyChanges(mDeviceContext.Get(), mDirectionalLightBuffer.Get(), mDirectionalLightParameters);
		mDeviceContext->PSSetShaderResources(3u, 1u, mDirectionalLightSRV.GetAddressOf());*/
	}

	for (auto actor : mRenderObjects)
	{
		actor->Draw(mTPCamera->GetViewMatrix(), mTPCamera->GetPerspectiveProjectionMatrix());
	}
}

void Graphics::RenderLighting()
{
	for (auto& light : mLights)
	{
		UpdateLightConstantBuffer(light);
		mDeviceContext->VSSetConstantBuffers(1u, 1u, mCB_Light.GetAddressOf());
		mDeviceContext->PSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());

		const auto lightType = light->GetLightType();
		if (lightType == ELightType::Directional)
		{
			pRenderer->BindOutsideFrustum();
			pRenderer->DrawScreenQuad();
		}
		else // omni or spot
		{
			if (lightType == ELightType::Point)
			{
				// TODO: here we could already have proper range and don't need to calculate it explicitly
				const auto scale = light->GetRange();

				mLightVolumeData.gWorld = XMMatrixTranspose(XMMatrixScaling(scale, scale, scale) * light->GetTransform()->mRotationMatrix * light->GetTransform()->mTranslationMatrix);
				mLightVolumeData.gView = XMMatrixTranspose(mTPCamera->GetViewMatrix());
				mLightVolumeData.gProjection = XMMatrixTranspose(mTPCamera->GetPerspectiveProjectionMatrix());
				mCB_LightVolume.SetAndApplyData(mLightVolumeData);
				mDeviceContext->VSSetConstantBuffers(0u, 1u, mCB_LightVolume.GetAddressOf());

				pRenderer->BindWithinFrustum();
				light->DrawLightVolume(mDeviceContext.Get());
				continue;
			}

			// @TODO: make a spot light
			if (lightType == ELightType::Spot)
			{
				continue;
			}
		}
	}
}

void Graphics::RenderParticles(float deltaTime)
{
	pRenderer->BindParticlesPass();
	pFireParticleSystem->Update(deltaTime);
	pFireParticleSystem->Render();
}

void Graphics::UpdateLightConstantBuffer(Light* light)
{
	const auto ligthType = light->GetLightType();

	mLightData.diffuse = light->GetDiffuseColor();
	mLightData.specular = light->GetSpecularColor();
	mLightData.lightType = ligthType;
	
	if (ligthType == ELightType::Directional)
	{
		mLightData.ambient = light->GetAmbientColor();
		mLightData.direction = light->GetDirection();
	}
	else // omni or spot
	{
		mLightData.attenuation = light->GetAttenuation();
		mLightData.position = light->GetPositionFloat();

		if (ligthType == ELightType::Point)
		{
			// TODO: move to point light update
			light->SetRange(CalcPointLightRange(*light));
			mLightData.range = light->GetRange(); // hard-coded value
		}
		else // spot
		{
			mLightData.direction = light->GetDirection();
			mLightData.spot = 10.0f; // hard-coded value
		}
	}
	mCB_Light.SetAndApplyData(mLightData);
}

void Graphics::EndFrame()
{
	// Step 15: At the End of While (!isExitRequested): Present the Result
	mDeviceContext->OMSetRenderTargets(0u, nullptr, nullptr);
	ThrowIfFailed(mSwapChain->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}

void Graphics::Update(const ScaldTimer& st)
{
	mTPCamera->Update(st);

	//pFireParticleSystem->Update(st.DeltaTime());
}

void Graphics::CreateDepthStencilState()
{
	pRenderer->CreateDepthStencilState();
}

void Graphics::CreateRasterizerState()
{
	pRenderer->CreateRasterizerState();
}

void Graphics::CreateSamplerState()
{
	pRenderer->CreateSamplerState();
}

void Graphics::CreateBlendState()
{
	pRenderer->CreateBlendState();
}

void Graphics::SetupShaders()
{
	pRenderer->SetupShaders();
}

void Graphics::InitPointLights()
{
	ThrowIfFailed(CreateStructuredBuffer(mDevice.Get(), mPointLightBuffer.GetAddressOf(), mPointLightsParameters));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0u;
	srvDesc.Buffer.NumElements = (UINT)mPointLightsParameters.size();

	ThrowIfFailed(mDevice->CreateShaderResourceView(mPointLightBuffer.Get(), &srvDesc, &mPointLightSRV));
}

void Graphics::InitSpotLights()
{
	ThrowIfFailed(CreateStructuredBuffer(mDevice.Get(), mSpotLightBuffer.GetAddressOf(), mSpotLightsParameters));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0u;
	srvDesc.Buffer.NumElements = (UINT)mSpotLightsParameters.size();

	ThrowIfFailed(mDevice->CreateShaderResourceView(mSpotLightBuffer.Get(), &srvDesc, &mSpotLightSRV));
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

void Graphics::GetLightSpaceMatrices(std::vector<XMMATRIX>& outMatrices)
{
	for (UINT i = 0; i < CASCADE_NUMBER; ++i)
	{
		if (i == 0)
		{
			outMatrices.push_back(GetLightSpaceMatrix(mCameraNearZ, mCascadeShadowMap->GetCascadeLevel(i)));
		}
		else if (i < CASCADE_NUMBER - 1)
		{
			outMatrices.push_back(GetLightSpaceMatrix(mCascadeShadowMap->GetCascadeLevel(i - 1), mCascadeShadowMap->GetCascadeLevel(i)));
		}
		else
		{
			outMatrices.push_back(GetLightSpaceMatrix(mCascadeShadowMap->GetCascadeLevel(i - 1), mCascadeShadowMap->GetCascadeLevel(i)));
		}
	}
}

XMMATRIX Graphics::GetLightSpaceMatrix(const float nearPlane, const float farPlane)
{
	const auto cameraProjectionMatrix = XMMatrixPerspectiveFovLH(mTPCamera->GetFovRad(), static_cast<float>(mScreenWidth) / static_cast<float>(mScreenHeight), nearPlane, farPlane);
	const auto frustumCorners = GetFrustumCornersWorldSpace(mTPCamera->GetViewMatrix() * cameraProjectionMatrix);

	XMVECTOR center = XMVectorZero();
	for (const auto& v : frustumCorners)
	{
		center += v;
	}

	center /= (float)frustumCorners.size();
	const XMFLOAT3 lightDir = mDirectionalLight->GetDirection();
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

// Deferred specific
float Graphics::CalcPointLightRange(const Light& light)
{
	const auto color = light.GetLightParams()->diffuse;
	const auto attenuation = light.GetLightParams()->attenuation;

	float MaxChannel = fmax(fmax(color.x, color.y), color.z);

	float ret = (-attenuation.y + sqrtf(attenuation.y * attenuation.y - 4 * attenuation.z * (attenuation.z - 256 * MaxChannel * color.w))) / (2 * attenuation.z);
	return ret;
}