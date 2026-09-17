#include "stdafx.h"
#include "ScaldException.h"

#include "Graphics.h"
#include "Camera/ThirdPersonCamera.h"
#include "Renderer/DeferredRenderer.h"
#include "Shadows/CascadeShadowMap.h"
#include "Particles/FireParticleSystem.h"
#include "Scene/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Scene/LightSceneProxy.h"

using namespace Scald;

namespace RenderCommon
{
    constexpr uint32_t kSwapChainBufferCount = 2u;
    constexpr DXGI_FORMAT kBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    constexpr DXGI_FORMAT kDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
}

Graphics::Graphics(HWND hWnd, int width, int height)
    : m_screenWidth(width)
    , m_screenHeight(height)
{
    D3D_FEATURE_LEVEL featureLevel[] = {D3D_FEATURE_LEVEL_11_1};

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = RenderCommon::kSwapChainBufferCount;
    swapDesc.BufferDesc.Width = m_screenWidth;
    swapDesc.BufferDesc.Height = m_screenHeight;
    swapDesc.BufferDesc.Format = RenderCommon::kBackBufferFormat;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60u;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1u;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = hWnd;
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    swapDesc.SampleDesc.Count = 1u;
    swapDesc.SampleDesc.Quality = 0u;

    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
        featureLevel, 1u, D3D11_SDK_VERSION, 
        &swapDesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext));

    m_camera = std::make_unique<ThirdPersonCamera>();
    // RTV and BackBuffer are created down here in Renderer
    pRenderer = std::make_unique<DeferredRenderer>(mSwapChain.Get(), mDevice.Get(), mDeviceContext.Get(), width, height);
    pFireParticleSystem = std::make_unique<FireParticleSystem>(mDevice.Get(), mDeviceContext.Get(), 4096, XMVectorSet(15.0f, 5.0f, 60.0f, 1.0f), m_camera.get());
    // to renderer probably
    mCascadeShadowMap = std::make_unique<CascadeShadowMap>(mDevice.Get(), 2048u, 2048u);
}

Graphics::~Graphics() {}

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
    m_camera->Reset(mFovDegrees, static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight), mCameraNearZ, mCameraFarZ);

    // for cascade shadows
    mCascadeShadowMap->UpdateShadowCascadeSplits(mCameraNearZ, mCameraFarZ);

    // constant buffers setup (Forward)
    // ThrowIfFailed(mCBVSPerFrame.Init(mDevice.Get(), mDeviceContext.Get()));

    // constant buffers setup for deferred rendering
    ThrowIfFailed(mCB_LightVolume.Init(mDevice.Get(), mDeviceContext.Get()));
    ThrowIfFailed(mCB_PerFrame.Init(mDevice.Get(), mDeviceContext.Get()));
    ThrowIfFailed(mCB_CSM.Init(mDevice.Get(), mDeviceContext.Get()));
    // Should be replaced with structured buffer for every light type or smth
    ThrowIfFailed(mCB_Light.Init(mDevice.Get(), mDeviceContext.Get()));
}

// Before rendering every frame we should clear render target view and depth stencil view
void Graphics::ClearBuffer(float r)
{
    pRenderer->ClearBuffer(r);
}

// Deferred rendering
void Graphics::DrawScene(Scene* scene)
{
    // TODO: assert
    if (!scene) return;

    ID3D11ShaderResourceView* nullSrv[3] = {nullptr, nullptr, nullptr};
    mDeviceContext->PSSetShaderResources(0u, 3u, nullSrv);

#pragma region ShadowMappingPass
    mCascadeShadowMap->BindDsvAndSetNullRenderTarget(mDeviceContext.Get());
    pRenderer->BindDepthOnlyPass();
    RenderDepthOnlyPass(scene);
#pragma endregion ShadowMappingPass

#pragma region DeferredGeometryPass
    pRenderer->BindGeometryPass();
    BindGeometryPassResources();
    RenderGeometry(scene);
#pragma endregion DeferredGeometryPass

#pragma region DeferredLightingPass
    pRenderer->BindLightingPass();
    BindLightingPassResources();
    RenderLighting(scene);
    // additional task to deferred
    RenderGBuffer();
#pragma endregion DeferredLightingPass

#pragma region ForwardPasses
    /*pRenderer->BindTransparentPass();
    RenderTransparent();*/

    RenderParticles();
#pragma endregion ForwardPasses
}

// For both Forward and Deferred
void Graphics::RenderDepthOnlyPass(Scene* scene)
{
    std::array<XMMATRIX, kCascadeNumber> lightSpaceMatrices;
    GetLightSpaceMatrices(lightSpaceMatrices);

    for (UINT i = 0; i < kCascadeNumber; i++)
    {
        mCSMData.ViewProj[i] = XMMatrixTranspose(lightSpaceMatrices[i]);
        mCSMData.distances[i] = mCascadeShadowMap->GetCascadeLevel(i);  // not used on GPU in Geometry shader, but still filled
    }

    mCB_CSM.SetAndApplyData(mCSMData);
    mDeviceContext->GSSetConstantBuffers(0u, 1u, mCB_CSM.GetAddressOf());

    for (auto&& renderItem : scene->GetPrimitives())
    {
        // skip lights
        renderItem->Draw();
    }

    mDeviceContext->ClearState();
}

void Graphics::BindGeometryPassResources()
{
    const auto& viewMat = m_camera->GetViewMatrix();
    const auto& projMat = m_camera->GetPerspectiveProjectionMatrix();
    mPerFrameData.gView = XMMatrixTranspose(viewMat);
    mPerFrameData.gProjection = XMMatrixTranspose(projMat);
    mPerFrameData.gViewProj = XMMatrixTranspose(viewMat * projMat);
    mPerFrameData.gEyePos = m_camera->GetPosition();

    mCB_PerFrame.SetAndApplyData(mPerFrameData);
    mDeviceContext->VSSetConstantBuffers(1u, 1u, mCB_PerFrame.GetAddressOf());
}

void Graphics::RenderGeometry(Scene* scene)
{
    for (auto&& renderItem : scene->GetPrimitives())
    {
        renderItem->Draw();
    }
    mDeviceContext->ClearState();
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

void Graphics::RenderLighting(Scene* scene)
{
    RenderDirectionalLight(scene);
    RenderOmniLight(scene);
    //RenderSpotLight(scene);

    mDeviceContext->ClearState();
}

void Graphics::RenderDirectionalLight(Scene* scene)
{
    UpdateDirectionalLights();

    for (auto& light : scene->GetLights())  // must be list only with spots
    {

    }

    mDeviceContext->VSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
    mDeviceContext->PSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());

    pRenderer->BindOutsideFrustum();
    pRenderer->DrawScreenQuad();
}

void Graphics::RenderOmniLight(Scene* scene)
{
    pRenderer->BindIntersectsFarPlane();  // !!! HACK (TO DRAW EVEN IF FRUSTUM INTERSECTS LIGHT VOLUME)
    // pRenderer->BindWithinFrustum(); // SHOULD BE INSTEAD

    UpdateOmniLightParams();

    for (auto& light : scene->GetLights())  // must be list only with spots
    {

    }

    /*const float sphereVolumeRadius = light->GetRange();
    XMMATRIX world = XMMatrixScalingFromVector(XMVectorReplicate(sphereVolumeRadius)) * XMMatrixRotationQuaternion(light->GetTransform()->GetOrientation()) *
                     XMMatrixTranslationFromVector(light->GetTransform()->GetPositionVector());

    auto det = XMMatrixDeterminant(world);
    XMMATRIX invTransWorld = XMMatrixInverse(&det, XMMatrixTranspose(world));

    mLightVolumeData.gWorld = XMMatrixTranspose(world);
    mLightVolumeData.gInvTransWorld = XMMatrixTranspose(invTransWorld);

    mCB_LightVolume.SetAndApplyData(mLightVolumeData);
    mDeviceContext->VSSetConstantBuffers(0u, 1u, mCB_LightVolume.GetAddressOf());

    light->DrawLightVolume(mDeviceContext.Get());*/
}

void Graphics::RenderSpotLight(Scene* scene)
{
    pRenderer->BindWithinFrustum();

    UpdateSpotLightParams();
    for (auto& light : scene->GetLights())  // must be list only with spots
    {
    }
}

void Graphics::RenderParticles()
{
    pRenderer->BindParticlesPass();
    pFireParticleSystem->Render();
    mDeviceContext->ClearState();
}

void Graphics::RenderGBuffer()
{
    pRenderer->DrawGBuffer();
    mDeviceContext->ClearState();
}

void Graphics::SwitchGBufferLayer(int layer)
{
    pRenderer->ChangeGBufferLayer(layer);
}

void Graphics::UpdateDirectionalLights()
{
    /*mLightData.ambient = dirLight->GetAmbientColor();
    mLightData.diffuse = dirLight->GetDiffuseColor();
    mLightData.specular = dirLight->GetSpecularColor();
    mLightData.direction = dirLight->GetDirection();
    mLightData.lightType = ELightType::Directional;*/
    mCB_Light.SetAndApplyData(mLightData);
}

void Graphics::UpdateOmniLightParams()
{
    //mLightData.diffuse = pointLight->GetDiffuseColor();
    //mLightData.specular = pointLight->GetSpecularColor();
    //mLightData.lightType = pointLight->GetLightType();

    //mLightData.attenuation = pointLight->GetAttenuation();
    //mLightData.position = pointLight->GetPositionFloat();

    //mLightData.range = pointLight->GetRange();  // hard-coded value

    mCB_Light.SetAndApplyData(mLightData);
    mDeviceContext->VSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
    mDeviceContext->PSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
}

void Graphics::UpdateSpotLightParams()
{
    //mLightData.diffuse = spotLight->GetDiffuseColor();
    //mLightData.specular = spotLight->GetSpecularColor();
    //mLightData.lightType = spotLight->GetLightType();

    //mLightData.attenuation = spotLight->GetAttenuation();
    //mLightData.position = spotLight->GetPositionFloat();

    //mLightData.direction = spotLight->GetDirection();
    //mLightData.spot = 10.0f;  // hard-coded value

    mCB_Light.SetAndApplyData(mLightData);
    mDeviceContext->VSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
    mDeviceContext->PSSetConstantBuffers(2u, 1u, mCB_Light.GetAddressOf());
}

void Graphics::EndFrame()
{
    mDeviceContext->OMSetRenderTargets(0u, nullptr, nullptr);
    ThrowIfFailed(mSwapChain->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}

void Graphics::Update(float deltaTime)
{
    m_camera->Tick(deltaTime);
    pFireParticleSystem->Update(deltaTime);
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

std::array<XMVECTOR, 8u> Graphics::GetFrustumCornersWorldSpace(const XMMATRIX& viewProjection)
{
    XMVECTOR det;
    const auto inv = XMMatrixInverse(&det, viewProjection);

    std::array<XMVECTOR, 8u> frustumCorners;

    for (UINT x = 0; x < 2; ++x)
    {
        for (UINT y = 0; y < 2; ++y)
        {
            for (UINT z = 0; z < 2; ++z)
            {
                // translate NDC coords to world space
                const XMVECTOR pt = XMVector4Transform(XMVectorSet(2.0f * x - 1.0f, 2.0f * y - 1.0f, (float)z, 1.0f), inv);
                frustumCorners[x * 4 + y * 2 + z] = (pt / XMVectorGetW(pt));
            }
        }
    }
    return frustumCorners;
}

void Graphics::GetLightSpaceMatrices(std::array<XMMATRIX, kCascadeNumber>& outMatrices)
{
    for (UINT i = 0; i < kCascadeNumber; ++i)
    {
        if (i == 0)
        {
            outMatrices[i] = GetLightSpaceMatrix(mCameraNearZ, mCascadeShadowMap->GetCascadeLevel(i));
        }
        else
        {
            outMatrices[i] = GetLightSpaceMatrix(mCascadeShadowMap->GetCascadeLevel(i - 1), mCascadeShadowMap->GetCascadeLevel(i));
        }
    }
}

XMMATRIX Graphics::GetLightSpaceMatrix(const float nearPlane, const float farPlane)
{
    const auto cameraProjectionMatrix = XMMatrixPerspectiveFovLH(m_camera->GetFovRad(), static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight), nearPlane, farPlane);
    const auto frustumCorners = GetFrustumCornersWorldSpace(m_camera->GetViewMatrix() * cameraProjectionMatrix);

    XMVECTOR center = XMVectorZero();
    for (const auto& v : frustumCorners)
    {
        center += v;
    }

    center /= (float)frustumCorners.size();
    //const XMFLOAT3 lightDir = mDirectionalLight->GetDirection();
    const XMFLOAT3 lightDir = { 1.0f, 1.0f, -1.0f }; // TODO: remove hard-coded value
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