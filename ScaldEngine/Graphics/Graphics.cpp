#include "stdafx.h"
#include "ScaldException.h"
#include "Graphics.h"

#include "Render/DeferredRenderer.h"
#include "Particles/FireParticleSystem.h"
#include "Camera/ThirdPersonCamera.h"
#include "Objects/Geometry/Actor.h"
#include "Light/Light.h"
#include "Shadows/CascadeShadowMap.h"

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

	mTPCamera = std::make_unique<ThirdPersonCamera>();
	// RTV and BackBuffer are created down here in Renderer
	pRenderer = std::make_unique<DeferredRenderer>(mSwapChain.Get(), mDevice.Get(), mDeviceContext.Get(), width, height);
	pFireParticleSystem = std::make_unique<FireParticleSystem>(mDevice.Get(), mDeviceContext.Get(), 4096, XMVectorSet(15.0f, 5.0f, 60.0f, 1.0f), mTPCamera.get());
	// to renderer probably
	mCascadeShadowMap = std::make_unique<CascadeShadowMap>(mDevice.Get(), 2048u, 2048u);
}

Graphics::~Graphics()
{
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

	// constant buffers setup (Forward)
	//ThrowIfFailed(mCBVSPerFrame.Init(mDevice.Get(), mDeviceContext.Get()));

	// constant buffers setup for deferred rendering
	ThrowIfFailed(mCB_LightVolume.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCB_PerFrame.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCB_CSM.Init(mDevice.Get(), mDeviceContext.Get()));
	ThrowIfFailed(mCB_Light.Init(mDevice.Get(), mDeviceContext.Get()));
}

void Graphics::InitSceneObjects()
{
	if (mRenderObjects.empty()) return; // assert or smth

	mTPCamera->SetTarget(mPlayer);
	
	for (auto&& sceneObject : mRenderObjects)
	{
		sceneObject->Init(mDevice.Get(), mDeviceContext.Get());
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
	BindGeometryPassResources();
	for (auto&& actor : mRenderObjects)
	{
		actor->Draw();
	}

	mDeviceContext->ClearState();

	pRenderer->BindLightingPass();
	BindLightingPassResources();
	RenderLighting();
	mDeviceContext->ClearState();
	// additional task to deferred
	RenderGBuffer();

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

	for (auto&& actor : mRenderObjects)
	{
		if (actor == mDirectionalLight) continue;
		actor->Draw();
	}
}

void Graphics::BindGeometryPassResources()
{
	const auto& viewMat = mTPCamera->GetViewMatrix();
	const auto& projMat = mTPCamera->GetPerspectiveProjectionMatrix();
	mPerFrameData.gView = XMMatrixTranspose(viewMat);
	mPerFrameData.gProjection = XMMatrixTranspose(projMat);
	mPerFrameData.gViewProj = XMMatrixTranspose(viewMat * projMat);
	mPerFrameData.gEyePos = mTPCamera->GetPosition();

	mCB_PerFrame.SetAndApplyData(mPerFrameData);
	mDeviceContext->VSSetConstantBuffers(1u, 1u, mCB_PerFrame.GetAddressOf());
}

void Graphics::BindLightingPassResources()
{
	mCB_CSM.SetAndApplyData(mCSMData);
	mCB_PerFrame.SetAndApplyData(mPerFrameData);

	mDeviceContext->VSSetConstantBuffers(1u, 1u, mCB_PerFrame.GetAddressOf());
	mDeviceContext->PSSetConstantBuffers(0u, 1u, mCB_CSM.GetAddressOf());
	mDeviceContext->PSSetConstantBuffers(1u, 1u, mCB_PerFrame.GetAddressOf());
	mDeviceContext->PSSetShaderResources(3u, 1u, mCascadeShadowMap->GetAddressOf());
}

void Graphics::RenderLighting()
{
	RenderDirectionalLight();
	RenderOmniLights();
	//RenderSpotLights();
}

void Graphics::RenderDirectionalLight()
{
	UpdateDirLightConstantBuffer(mDirectionalLight.get());
	mDeviceContext->VSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
	mDeviceContext->PSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());

	pRenderer->BindOutsideFrustum();
	pRenderer->DrawScreenQuad();
}

void Graphics::RenderOmniLights()
{
	pRenderer->BindIntersectsFarPlane(); // !!! HACK (TO DRAW EVEN IF FRUSTUM INTERSECTS LIGHT VOLUME)
	//pRenderer->BindWithinFrustum(); // SHOULD BE INSTEAD

	for (auto&& light : mLights)
	{
		UpdateOmniLightConstantBuffer(light.get());

		const float sphereVolumeRadius = light->GetRange();
		XMMATRIX world = XMMatrixScalingFromVector(XMVectorReplicate(sphereVolumeRadius)) * light->GetTransform()->mRotationMatrix * light->GetTransform()->mTranslationMatrix;

		auto det = XMMatrixDeterminant(world);
		XMMATRIX invTransWorld = XMMatrixInverse(&det, XMMatrixTranspose(world));

		mLightVolumeData.gWorld = XMMatrixTranspose(world);
		mLightVolumeData.gInvTransWorld = XMMatrixTranspose(invTransWorld);

		mCB_LightVolume.SetAndApplyData(mLightVolumeData);
		mDeviceContext->VSSetConstantBuffers(0u, 1u, mCB_LightVolume.GetAddressOf());

		light->DrawLightVolume(mDeviceContext.Get());
	}
}

void Graphics::RenderSpotLights()
{
	pRenderer->BindWithinFrustum();

	for (auto& light : mLights) // must be list only with spots
	{
		UpdateSpotLightConstantBuffer(light.get());
	}
}

void Graphics::RenderParticles(float deltaTime)
{
	pRenderer->BindParticlesPass();
	pFireParticleSystem->Update(deltaTime);
	pFireParticleSystem->Render();
}

void Graphics::RenderGBuffer()
{
	pRenderer->DrawGBuffer();
}

void Graphics::SwitchGBufferLayer(int layer)
{
	pRenderer->ChangeGBufferLayer(layer);
}

void Graphics::UpdateDirLightConstantBuffer(Light* dirLight)
{
	mLightData.ambient = dirLight->GetAmbientColor();
	mLightData.diffuse = dirLight->GetDiffuseColor();
	mLightData.specular = dirLight->GetSpecularColor();
	mLightData.direction = dirLight->GetDirection();
	mLightData.lightType = ELightType::Directional;
	mCB_Light.SetAndApplyData(mLightData);
}

void Graphics::UpdateOmniLightConstantBuffer(Light* pointLight)
{
	mLightData.diffuse = pointLight->GetDiffuseColor();
	mLightData.specular = pointLight->GetSpecularColor();
	mLightData.lightType = pointLight->GetLightType();;

	mLightData.attenuation = pointLight->GetAttenuation();
	mLightData.position = pointLight->GetPositionFloat();

	mLightData.range = pointLight->GetRange(); // hard-coded value

	mCB_Light.SetAndApplyData(mLightData);
	mDeviceContext->VSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
	mDeviceContext->PSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
}

void Graphics::UpdateSpotLightConstantBuffer(Light* spotLight)
{
	mLightData.diffuse = spotLight->GetDiffuseColor();
	mLightData.specular = spotLight->GetSpecularColor();
	mLightData.lightType = spotLight->GetLightType();;

	mLightData.attenuation = spotLight->GetAttenuation();
	mLightData.position = spotLight->GetPositionFloat();

	mLightData.direction = spotLight->GetDirection();
	mLightData.spot = 10.0f; // hard-coded value

	mCB_Light.SetAndApplyData(mLightData);
	mDeviceContext->VSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
	mDeviceContext->PSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
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