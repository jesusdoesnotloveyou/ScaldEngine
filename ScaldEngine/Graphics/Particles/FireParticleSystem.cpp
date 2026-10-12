#include "stdafx.h"
#include "FireParticleSystem.h"
#include "Graphics/Camera/Camera.h"

using namespace Scald;

namespace
{
    const wchar_t* kFireParticlesVertexShaderPath           = L"./Data/Shaders/particles/fire/ParticlesVS.hlsl";
    const wchar_t* kFireParticlesPixelShaderPath            = L"./Data/Shaders/particles/fire/ParticlesPS.hlsl";
    const wchar_t* kFireParticlesSimulateComputeShaderPath  = L"./Data/Shaders/particles/fire/SimulateCS.hlsl";
    const wchar_t* kFireParticlesEmitComputeShaderPath      = L"./Data/Shaders/particles/fire/EmitCS.hlsl";
}

FireParticleSystem::FireParticleSystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int maxParticles, XMVECTOR origin, Camera* camera)
    : Super(device, deviceContext, maxParticles, origin, camera) {}

void FireParticleSystem::Update(float elapsedTime)
{
    Emit(16);
    Simulate(elapsedTime);
    SortParticles();
}

void FireParticleSystem::Render()
{
    Super::Render();

    mCameraData.gView = XMMatrixTranspose(camera->GetViewMatrix());
    mCameraData.gProjection = XMMatrixTranspose(camera->GetPerspectiveProjectionMatrix());
    mCameraData.gForward = camera->GetForwardVector();
    mCameraData.gUp = camera->GetUpVector();
    mCBCamera.SetAndApplyData(mCameraData);

    m_deviceContext->CopyStructureCount(indirectArgsBuffer, 0u, mSortListBufferUAV.Get());

    m_deviceContext->IASetInputLayout(mFireVertexShader.GetInputLayout());
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

    m_deviceContext->VSSetShaderResources(0u, 1u, mSortListBufferSRV.GetAddressOf());
    m_deviceContext->VSSetShader(mFireVertexShader.Get(), nullptr, 0u);

    m_deviceContext->GSSetShaderResources(0u, 1u, mParticleBufferSRV.GetAddressOf());
    m_deviceContext->GSSetShader(mBillboardGeometryShader.Get(), nullptr, 0u);
    m_deviceContext->GSSetConstantBuffers(0u, 1u, mCBCamera.GetAddressOf());

    m_deviceContext->PSSetShader(mFirePixelShader.Get(), nullptr, 0u);

    m_deviceContext->DrawInstancedIndirect(indirectArgsBuffer, 0u);
}

static bool firstEmit = true;
void FireParticleSystem::Emit(int numParticles)
{
    Super::Emit(numParticles);

    ApplyChanges(m_deviceContext, injectionBuffer, injectionBufferSize, injectionParticleData.get());

    mParticleData.maxNumParticles = m_maxParticles;
    mParticleData.numEmitInThisFrame = numParticles;
    mParticleData.gEmitPos = m_emitterOrigin;
    mParticleData.gEyePos = camera->GetPosition();

    if (firstEmit) mParticleData.numAliveParticles = 0u;
    mCBParticle.SetAndApplyData(mParticleData);

    if (!firstEmit)
    {
        m_deviceContext->CopyStructureCount(mCBParticle.Get(), 12, mSortListBufferUAV.Get());
    }

    UINT minOne = (UINT)-1;
    UINT zero = 0u;

    m_deviceContext->CSSetShader(mFireEmitShader.Get(), nullptr, 0u);
    m_deviceContext->CSSetUnorderedAccessViews(0u, 1u, mParticleBufferUAV.GetAddressOf(), nullptr);

    if (firstEmit)
    {
        m_deviceContext->CSSetUnorderedAccessViews(1u, 1u, mSortListBufferUAV.GetAddressOf(), &zero);
        m_deviceContext->CSSetUnorderedAccessViews(2u, 1u, mDeadListBufferUAV.GetAddressOf(), &m_maxParticles);
        firstEmit = false;
    }
    else
    {
        m_deviceContext->CSSetUnorderedAccessViews(1u, 1u, mSortListBufferUAV.GetAddressOf(), &minOne);
        m_deviceContext->CSSetUnorderedAccessViews(2u, 1u, mDeadListBufferUAV.GetAddressOf(), &minOne);
    }

    m_deviceContext->CSSetConstantBuffers(0u, 1u, mCBParticle.GetAddressOf());
    m_deviceContext->CSSetShaderResources(0u, 1u, mInjectionBufferSRV.GetAddressOf());
    m_deviceContext->Dispatch(1u, 1u, 1u);

    ID3D11UnorderedAccessView* nullUAV = nullptr;
    m_deviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, nullptr);
    m_deviceContext->CSSetUnorderedAccessViews(1u, 1u, &nullUAV, nullptr);
    m_deviceContext->CSSetUnorderedAccessViews(2u, 1u, &nullUAV, nullptr);
    mParticleData.numEmitInThisFrame = 0u;
}

void FireParticleSystem::InitializeSystem()
{
    Super::InitializeSystem();

    D3D11_INPUT_ELEMENT_DESC inputLayoutParticlesDesc[] = {D3D11_INPUT_ELEMENT_DESC{"SV_VertexID", 0u, DXGI_FORMAT_R32_UINT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}};

    ThrowIfFailed(mFireVertexShader.Init(m_device, inputLayoutParticlesDesc, (UINT)std::size(inputLayoutParticlesDesc), kFireParticlesVertexShaderPath));
    ThrowIfFailed(mFirePixelShader.Init(m_device, kFireParticlesPixelShaderPath));
    ThrowIfFailed(mFireSimulateShader.Init(m_device, kFireParticlesSimulateComputeShaderPath));
    ThrowIfFailed(mFireEmitShader.Init(m_device, kFireParticlesEmitComputeShaderPath));
}

FireParticleSystem::~FireParticleSystem() noexcept {}

void FireParticleSystem::InitializeParticle(int index)
{
    if (index < 0 || index >= injectionBufferSize) return;

    Particle p{};
    p.acceleration = XMVectorSet(0.0f, 4.0f * -0.981f, 0.0f, 0.0f);
    p.initialColor = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    p.endColor = XMVectorSet(1.0f, 1.0f, 0.8f, 1.0f);  // randColor();
    p.initialSize = 0.02f;
    p.endSize = 0.04f;
    p.initialWeight = 1.0f;
    p.endWeight = 1.0f;
    p.lifeTime = 0.0f;
    p.maxLifeTime = 3.0f;
    p.pos = m_emitterOrigin;
    p.prevPos = m_emitterOrigin;

    float rf = generateRandom(0.0f, 1.0f * XM_2PI);
    p.velocity = generateRandom(0.3f, 2.0f) * XMVectorSet(std::sin(rf), generateRandom(0.0f, 1.0f), std::cos(rf), 0.0f);

    injectionParticleData[index] = p;
}

void FireParticleSystem::Simulate(float elapsedTime)
{
    mParticleData.deltaTime = elapsedTime;
    mParticleData.maxNumParticles = m_maxParticles;
    mParticleData.gEyePos = camera->GetPosition();
    mCBParticle.SetAndApplyData(mParticleData);

    // numAliveParticles starts from 12th byte
    m_deviceContext->CopyStructureCount(mCBParticle.Get(), 12u, mSortListBufferUAV.Get());

    UINT minOne = (UINT)-1;
    UINT zero = 0;

    m_deviceContext->CSSetUnorderedAccessViews(0u, 1u, mParticleBufferUAV.GetAddressOf(), nullptr);
    m_deviceContext->CSSetUnorderedAccessViews(1u, 1u, mSortListBufferUAV.GetAddressOf(), &minOne);
    m_deviceContext->CSSetUnorderedAccessViews(2u, 1u, mDeadListBufferUAV.GetAddressOf(), &minOne);

    m_deviceContext->CSSetShader(mFireSimulateShader.Get(), nullptr, 0u);
    m_deviceContext->CSSetConstantBuffers(0u, 1u, mCBParticle.GetAddressOf());
    m_deviceContext->Dispatch(32u, 32u, 1u);

    ID3D11UnorderedAccessView* nullUAV = nullptr;
    m_deviceContext->CSSetUnorderedAccessViews(0u, 1u, &nullUAV, nullptr);
    m_deviceContext->CSSetUnorderedAccessViews(1u, 1u, &nullUAV, nullptr);
    m_deviceContext->CSSetUnorderedAccessViews(2u, 1u, &nullUAV, nullptr);
}