#include "stdafx.h"
#include "ScaldException.h"

#include "Graphics.h"
#include "Model.h"
#include "Camera/ThirdPersonCamera.h"
#include "Renderer/DeferredRenderer.h"
#include "Shadows/CascadeShadowMap.h"
#include "Particles/FireParticleSystem.h"
#include "Scene/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Scene/LightSceneProxy.h"

#include <cstdio>

using namespace Scald;

namespace RenderCommon
{
    constexpr uint32_t kSwapChainBufferCount = 2u;
    constexpr DXGI_FORMAT kBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    constexpr DXGI_FORMAT kDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    constexpr uint32_t kCascadesShadowMapResolution = 2048u;
}
// D3D Initialization:
/*
    Factory creation (1 or 6)
    Iterate through adapters in the system
    Create device
    Create device context
    Create swap chain
*/
ComPtr<IDXGIAdapter1> Graphics::GetHighPerformanceAdapter(IDXGIFactory1* pBaseFactory)
{
    // To use EnumAdapterByGpuPreference
    ComPtr<IDXGIFactory6> factory6;
    ComPtr<IDXGIAdapter1> adapter;

    if (SUCCEEDED(pBaseFactory->QueryInterface(__uuidof(IDXGIFactory6), reinterpret_cast<void**>(factory6.GetAddressOf()))))
    {
        for (uint32_t index = 0u; 
            factory6->EnumAdapterByGpuPreference(
                index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND;
            ++index)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);
            LogDeviceAdapter(desc);       

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
            return adapter;
        }
    }
    return nullptr;
}

void Graphics::LogDeviceAdapter(const DXGI_ADAPTER_DESC1& desc) const
{
    wchar_t msg[256];
    swprintf_s(msg, L"D3D11 device created on: %s | VendorId: 0x%04X | VRAM: %zu MB | LUID: %08lX:%08lX | FL: 0x%X\n", 
        desc.Description, desc.VendorId, desc.DedicatedVideoMemory / (1024 * 1024), 
        desc.AdapterLuid.HighPart, desc.AdapterLuid.LowPart, m_featureLevel);
    OutputDebugStringW(msg);
}

Graphics::Graphics(HWND hWnd, int width, int height)
    : m_screenWidth(width)
    , m_screenHeight(height)
{
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,  // GPU
        D3D_DRIVER_TYPE_WARP,      // Software rasterizer on CPU?  
        D3D_DRIVER_TYPE_REFERENCE, // Cucumber
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    UINT creationFlags = 0u;

#if defined(_DEBUG) || defined(DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = S_OK;

    ComPtr<IDXGIFactory1> dxgiFactory1;
    ComPtr<IDXGIAdapter1> adapter1;

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> immediateContext;

    // Create device and swapchain separately since it is modern and flexible approach
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory1))))
    {
        if (adapter1 = GetHighPerformanceAdapter(dxgiFactory1.Get()))
        {
            m_driverType = D3D_DRIVER_TYPE_UNKNOWN;

            hr = D3D11CreateDevice(adapter1.Get(), m_driverType, nullptr,
                creationFlags, featureLevels, numFeatureLevels, 
                D3D11_SDK_VERSION, &device, &m_featureLevel, &immediateContext);
        }
        
        if (FAILED(hr))
        {
            for (UINT driverIndex = 0u; driverIndex < numDriverTypes; driverIndex++)
            {
                m_driverType = driverTypes[driverIndex];
                hr = D3D11CreateDevice(nullptr, m_driverType, nullptr,
                    creationFlags, featureLevels, numFeatureLevels,
                    D3D11_SDK_VERSION, &device, &m_featureLevel, &immediateContext);

                if (SUCCEEDED(hr)) break;
            }
        }
    }

    ComPtr<IDXGIFactory2> dxgiFactory2;
    if (SUCCEEDED(dxgiFactory1.As(&dxgiFactory2)))
    {
        if (SUCCEEDED(device.As(&m_device1)) &&
            SUCCEEDED(immediateContext.As(&m_immediateContext1)))
        {
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc1 = {};
            swapChainDesc1.Width = width;
            swapChainDesc1.Height = height;
            swapChainDesc1.Format = RenderCommon::kBackBufferFormat;
            swapChainDesc1.SampleDesc.Count = 1u;
            swapChainDesc1.SampleDesc.Quality = 0u;
            swapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc1.BufferCount = RenderCommon::kSwapChainBufferCount;
            swapChainDesc1.Scaling = DXGI_SCALING_NONE;
            swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            swapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc = {};
            fullscreenDesc.RefreshRate.Numerator = 60u;
            fullscreenDesc.RefreshRate.Denominator = 1u;
            fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            fullscreenDesc.Windowed = TRUE;
        
            hr = dxgiFactory2->CreateSwapChainForHwnd(m_device1.Get(), hWnd, 
                &swapChainDesc1, &fullscreenDesc, nullptr, &m_swapChain1);
        }
    }
    else
    {
        hr = E_FAIL;
        ThrowIfFailed(hr);
    }

    m_camera = std::make_unique<Camera>();
    // RTV and BackBuffer are created down here in Renderer
    m_renderer = std::make_unique<DeferredRenderer>(m_swapChain1.Get(), m_device1.Get(), m_immediateContext1.Get(), width, height);
    m_fireParticleSystem = std::make_unique<FireParticleSystem>(m_device1.Get(), m_immediateContext1.Get(), 4096, XMVectorSet(15.0f, 5.0f, 60.0f, 1.0f), m_camera.get());
    // to renderer probably
    m_cascadeShadowMap = std::make_unique<CascadeShadowMap>(m_device1.Get(), RenderCommon::kCascadesShadowMapResolution, RenderCommon::kCascadesShadowMapResolution);
}

Graphics::~Graphics() {}

void Graphics::Setup()
{
    m_renderer->SetupShaders();
    m_renderer->CreateDepthStencilState();
    m_renderer->CreateRasterizerState();
    m_renderer->CreateSamplerState();
    m_renderer->CreateBlendState();

    // Camera setup
    m_camera->Reset(m_fovDegrees, static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight), m_cameraNearZ, m_cameraFarZ);

    // Particles
    m_fireParticleSystem->InitializeSystem();

    // Cascade shadows
    m_cascadeShadowMap->UpdateShadowCascadeSplits(m_cameraNearZ, m_cameraFarZ);

    // Constant buffers setup for deferred rendering
    ThrowIfFailed(m_lightVolumeCB.Init(m_device1.Get(), m_immediateContext1.Get()));
    ThrowIfFailed(m_perObjectCB.Init(m_device1.Get(), m_immediateContext1.Get()));
    ThrowIfFailed(m_perFrameCB.Init(m_device1.Get(), m_immediateContext1.Get()));
    ThrowIfFailed(m_csmCB.Init(m_device1.Get(), m_immediateContext1.Get()));
    // Should be replaced with structured buffer for every light type or smth
    ThrowIfFailed(m_lightCB.Init(m_device1.Get(), m_immediateContext1.Get()));
}

// Before rendering every frame we should clear render target view and depth stencil view
void Graphics::ClearBuffer(float r)
{
    m_renderer->ClearBuffer(r);
}

// Deferred rendering
void Graphics::DrawScene(Scene* scene)
{
    assert(scene != nullptr && "Scene is not valid");

    ID3D11ShaderResourceView* nullSrv[3] = {nullptr, nullptr, nullptr};
    m_immediateContext1->PSSetShaderResources(0u, 3u, nullSrv);

#pragma region ShadowMappingPass
    m_cascadeShadowMap->BindDsvAndSetNullRenderTarget(m_immediateContext1.Get());
    m_renderer->BindDepthOnlyPass();
    RenderDepthOnlyPass(scene);
#pragma endregion ShadowMappingPass

#pragma region DeferredGeometryPass
    m_renderer->BindGeometryPass();
    BindPerFrameResources();
    RenderGeometry(scene);
#pragma endregion DeferredGeometryPass

#pragma region DeferredLightingPass
    m_renderer->BindLightingPass();
    BindLightingPassResources();
    RenderLighting(scene);
    // additional task to deferred
    RenderGBuffer();
#pragma endregion DeferredLightingPass

#pragma region ForwardPasses
    /*m_renderer->BindTransparentPass();
    RenderTransparent();*/
    RenderParticles();
#pragma endregion ForwardPasses
}

// For both Forward and Deferred
void Graphics::RenderDepthOnlyPass(Scene* scene)
{
    std::array<XMMATRIX, kCascadeNumber> lightSpaceMatrices;
    GetLightSpaceMatrices(lightSpaceMatrices);

    CascadeShadowsConstants csmCB = {};

    for (UINT i = 0; i < kCascadeNumber; i++)
    {
        csmCB.ViewProj[i] = XMMatrixTranspose(lightSpaceMatrices[i]);
        csmCB.Distance[i] = m_cascadeShadowMap->GetCascadeLevel(i);  // not used on GPU in Geometry shader, but still filled
    }

    m_csmCB.SetAndApplyData(csmCB);
    m_immediateContext1->GSSetConstantBuffers(0u, 1u, m_csmCB.GetAddressOf());

    RenderGeometry(scene);
}

void Graphics::BindPerFrameResources()
{
    // TODO: UpdatePerFrameCB() call
    PerFrameConstants perFrameCB = {};

    const auto& viewMat = m_camera->GetViewMatrix();
    const auto& projMat = m_camera->GetPerspectiveProjectionMatrix();
    perFrameCB.View = XMMatrixTranspose(viewMat);
    perFrameCB.Proj = XMMatrixTranspose(projMat);
    perFrameCB.ViewProj = XMMatrixTranspose(viewMat * projMat);
    perFrameCB.EyePos = m_camera->GetPositionFloat();

    m_perFrameCB.SetAndApplyData(perFrameCB);
    m_immediateContext1->VSSetConstantBuffers(1u, 1u, m_perFrameCB.GetAddressOf());
}

void Graphics::RenderGeometry(Scene* scene)
{
    for (const auto& renderItem : scene->GetPrimitives())
    {
        UpdateObjectCB(renderItem->GetWorld());
        // Ordering is important: first texture, then model, because model will be drawn with this texture
        DrawTexture(renderItem->GetTexture());
        DrawModel(renderItem->GetModel());
    }
    ClearContext();
}

void Graphics::BindLightingPassResources()
{
    // TODO: check are there constant buffers from previous subpasses or we should set them again
    //m_csmCB.SetAndApplyData(csmData);
    //m_perFrameCB.SetAndApplyData(perFrameData);

    m_immediateContext1->VSSetConstantBuffers(1u, 1u, m_perFrameCB.GetAddressOf());
    m_immediateContext1->PSSetConstantBuffers(0u, 1u, m_csmCB.GetAddressOf());
    m_immediateContext1->PSSetConstantBuffers(1u, 1u, m_perFrameCB.GetAddressOf());
    m_immediateContext1->PSSetShaderResources(3u, 1u, m_cascadeShadowMap->GetAddressOf());
}

void Graphics::RenderLighting(Scene* scene)
{
    RenderDirectionalLight(scene);
    RenderOmniLight(scene);
    //RenderSpotLight(scene);

    ClearContext();
}

void Graphics::RenderDirectionalLight(Scene* scene)
{
    UpdateDirectionalLights();

    for (auto& light : scene->GetLights())  // must be list only with spots
    {

    }

    m_immediateContext1->VSSetConstantBuffers(2u, 1u, m_lightCB.GetAddressOf());
    m_immediateContext1->PSSetConstantBuffers(2u, 1u, m_lightCB.GetAddressOf());

    m_renderer->BindOutsideFrustum();
    m_renderer->DrawScreenQuad();
}

void Graphics::RenderOmniLight(Scene* scene)
{
    m_renderer->BindIntersectsFarPlane();  // !!! HACK (TO DRAW EVEN IF FRUSTUM INTERSECTS LIGHT VOLUME)
    // m_renderer->BindWithinFrustum(); // SHOULD BE INSTEAD

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
    mLightVolumeData.gInvTransposeWorld = XMMatrixTranspose(invTransWorld);

    m_lightVolumeCB.SetAndApplyData(mLightVolumeData);
    m_immediateContext1->VSSetConstantBuffers(0u, 1u, m_lightVolumeCB.GetAddressOf());

    light->DrawLightVolume(m_immediateContext1.Get());*/
}

void Graphics::RenderSpotLight(Scene* scene)
{
    m_renderer->BindWithinFrustum();

    UpdateSpotLightParams();
    for (auto& light : scene->GetLights())  // must be list only with spots
    {

    }
}

void Graphics::ClearContext()
{
    m_immediateContext1->ClearState();
}

void Graphics::UpdateObjectCB(const XMMATRIX& world)
{
    PerObjectConstants perObjectVertexShaderCB = {};

    auto det = XMMatrixDeterminant(world);
    const XMMATRIX invTransWorld = XMMatrixInverse(&det, XMMatrixTranspose(world));

    perObjectVertexShaderCB.World = XMMatrixTranspose(world);
    perObjectVertexShaderCB.InvTransposeWorld = XMMatrixTranspose(invTransWorld);

    m_perObjectCB.SetAndApplyData(perObjectVertexShaderCB);
}

void Graphics::UpdatePerFrameCB()
{

}

void Graphics::RenderParticles()
{
    m_renderer->BindParticlesPass();
    m_fireParticleSystem->Render();
    ClearContext();
}

void Graphics::RenderGBuffer()
{
    m_renderer->DrawGBuffer();
    ClearContext();
}

void Graphics::DrawModel(const Model* modelToDraw)
{
    assert(modelToDraw && "Static mesh is invalid");

    m_immediateContext1->VSSetConstantBuffers(0u, 1u, m_perObjectCB.GetAddressOf());
        
    for (const auto& mesh : modelToDraw->GetMeshes())
    {
        const auto& vertexBuffer = mesh.GetVertexBuffer();
        const auto& indexBuffer = mesh.GetIndexBuffer();

        m_immediateContext1->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), vertexBuffer.GetStridePtr(), vertexBuffer.GetOffsetPtr());
        m_immediateContext1->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
        [[likely]]
        if (indexBuffer.GetBufferSize())
        {
            m_immediateContext1->DrawIndexed(indexBuffer.GetBufferSize(), 0u, 0);
        }
        else
        {
            m_immediateContext1->Draw(vertexBuffer.GetBufferSize(), 0u);
        }
    }
}

void Graphics::DrawTexture(const Texture* textureView)
{
    m_immediateContext1->PSSetShaderResources(0u, 1u, textureView->Srv.GetAddressOf());
}

void Graphics::SwitchGBufferLayer(int layer)
{
    m_renderer->ChangeGBufferLayer(layer);
}

void Graphics::UpdateDirectionalLights()
{
    LIGHT_DESC dirLightCB = {};
    dirLightCB.ambient = XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f};
    /*mLightData.diffuse = dirLight->GetDiffuseColor();
    mLightData.specular = dirLight->GetSpecularColor();
    mLightData.direction = dirLight->GetDirection();*/
    dirLightCB.lightType = ELightType::Directional;
    m_lightCB.SetAndApplyData(dirLightCB);
}

void Graphics::UpdateOmniLightParams()
{
    LIGHT_DESC omniLightCB = {};
    //mLightData.diffuse = pointLight->GetDiffuseColor();
    //mLightData.specular = pointLight->GetSpecularColor();
    omniLightCB.lightType = ELightType::Point;

    //mLightData.attenuation = pointLight->GetAttenuation();
    //mLightData.position = pointLight->GetPositionFloat();

    //mLightData.range = pointLight->GetRange();  // hard-coded value

    m_lightCB.SetAndApplyData(omniLightCB);
    m_immediateContext1->VSSetConstantBuffers(2u, 1u, m_lightCB.GetAddressOf());
    m_immediateContext1->PSSetConstantBuffers(2u, 1u, m_lightCB.GetAddressOf());
}

void Graphics::UpdateSpotLightParams()
{
    LIGHT_DESC spotLightCB = {};
    //mLightData.diffuse = spotLight->GetDiffuseColor();
    //mLightData.specular = spotLight->GetSpecularColor();
    spotLightCB.lightType = ELightType::Spot;

    //mLightData.attenuation = spotLight->GetAttenuation();
    //mLightData.position = spotLight->GetPositionFloat();

    //mLightData.direction = spotLight->GetDirection();
    //mLightData.spot = 10.0f;  // hard-coded value

    m_lightCB.SetAndApplyData(spotLightCB);
    m_immediateContext1->VSSetConstantBuffers(2u, 1u, m_lightCB.GetAddressOf());
    m_immediateContext1->PSSetConstantBuffers(2u, 1u, m_lightCB.GetAddressOf());
}

void Graphics::EndFrame()
{
    m_immediateContext1->OMSetRenderTargets(0u, nullptr, nullptr);
    ThrowIfFailed(m_swapChain1->Present(1u, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0u));
}

void Graphics::Update(float deltaTime)
{
    m_camera->Tick(deltaTime);
    m_fireParticleSystem->Update(deltaTime);
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
            outMatrices[i] = GetLightSpaceMatrix(m_cameraNearZ, m_cascadeShadowMap->GetCascadeLevel(i));
        }
        else
        {
            outMatrices[i] = GetLightSpaceMatrix(m_cascadeShadowMap->GetCascadeLevel(i - 1), m_cascadeShadowMap->GetCascadeLevel(i));
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