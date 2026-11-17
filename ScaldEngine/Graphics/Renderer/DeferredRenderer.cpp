#include "stdafx.h"
#include "DeferredRenderer.h"
#include "Graphics/ScaldCoreTypes.h"
#include "ScaldException.h"

using namespace Scald;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace
{
    const wchar_t* kOpaqueVertexShaderPath                   = L"./Data/Shaders/DeferredOpaqueVS.hlsl";
    const wchar_t* kOpaquePixelShaderPath                    = L"./Data/Shaders/DeferredOpaquePS.hlsl";
    const wchar_t* kDeferredLightingVertexShaderPath         = L"./Data/Shaders/DeferredLightingDirectVS.hlsl";
    const wchar_t* kDeferredLightingPixelShaderPath          = L"./Data/Shaders/DeferredLightingDirectPS.hlsl";
    const wchar_t* kDeferredLightingAnalyticVertexShaderPath = L"./Data/Shaders/DeferredLightingAnalyticVS.hlsl";
    const wchar_t* kDeferredLightingAnalyticPixelShaderPath  = L"./Data/Shaders/DeferredLightingAnalyticPS.hlsl";
    const wchar_t* kDrawGBufferVertexShaderPath              = L"./Data/Shaders/GBufferVS.hlsl";
    const wchar_t* kDrawGBufferPixelShaderPath               = L"./Data/Shaders/GBufferPS.hlsl";
    const wchar_t* kPBRUberPixelShaderPath                   = L"./Data/Shaders/PBRUberPS.hlsl";
    }  // namespace

DeferredRenderer::DeferredRenderer(IDXGISwapChain1* swapChain, ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, UINT width, UINT height)
    : Super(swapChain, device, deviceContext, width, height)
{
    D3D11_TEXTURE2D_DESC textureDesc = {};
    ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1u;
    textureDesc.ArraySize = 1u;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1u;
    textureDesc.SampleDesc.Quality = 0u;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0u;
    textureDesc.MiscFlags = 0u;

    // back buffer I suppose
    for (UINT i = 0; i < kGBufferTexturesCount; i++)
    {
        ThrowIfFailed(device->CreateTexture2D(&textureDesc, nullptr, &mGBuffer.texture[i]));
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
    ZeroMemory(&textureDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    for (UINT i = 0; i < kGBufferTexturesCount; i++)
    {
        ThrowIfFailed(device->CreateRenderTargetView(mGBuffer.texture[i].Get(), &renderTargetViewDesc, &mGBuffer.rtv[i]));
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    ZeroMemory(&textureDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1u;

    for (UINT i = 0; i < kGBufferTexturesCount; i++)
    {
        ThrowIfFailed(device->CreateShaderResourceView(mGBuffer.texture[i].Get(), &shaderResourceViewDesc, &mGBuffer.srv[i]));
    }
}

DeferredRenderer::~DeferredRenderer() noexcept {}

void DeferredRenderer::SetupShaders()
{
    Super::SetupShaders();

    D3D11_INPUT_ELEMENT_DESC inputLayoutOpaqueDesc[] = {D3D11_INPUT_ELEMENT_DESC{"POSITION", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        D3D11_INPUT_ELEMENT_DESC{"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        D3D11_INPUT_ELEMENT_DESC{"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u}};

    D3D11_INPUT_ELEMENT_DESC inputLayoutQuadDesc[] = {D3D11_INPUT_ELEMENT_DESC{"POSITION", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u}};

    mGBufferVS.Init(m_device, kDrawGBufferVertexShaderPath, inputLayoutQuadDesc, (UINT)std::size(inputLayoutQuadDesc));
    mGBufferPS.Init(m_device, kDrawGBufferPixelShaderPath);

    m_opaqueVertexShader.Init(m_device, kOpaqueVertexShaderPath, inputLayoutOpaqueDesc, (UINT)std::size(inputLayoutOpaqueDesc));
    m_opaquePixelShader.Init(m_device, kOpaquePixelShaderPath);

    m_lightingVertexShader.Init(m_device, kDeferredLightingVertexShaderPath, inputLayoutQuadDesc, (UINT)std::size(inputLayoutQuadDesc));
    m_lightingPixelShader.Init(m_device, kDeferredLightingPixelShaderPath);

    const D3D_SHADER_MACRO pPBRDefines[] = {
        //{ "IBL", "1"},
        { "USE_NORMAL_MAP", "1"},
        { "USE_METALLIC_MAP", "1"},
        { "USE_ROUGHNESS_MAP", "1"},
        { "USE_AO_MAP", "1"},
        { nullptr, nullptr } // sentinel
    };

    m_PBRUberPixelShader.Init(m_device, kPBRUberPixelShaderPath, pPBRDefines);
}

void DeferredRenderer::CreateRasterizerState()
{
    Super::CreateRasterizerState();
}

void DeferredRenderer::BindGeometryPass()
{
    m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->IASetInputLayout(m_opaqueVertexShader.GetInputLayout());

    ID3D11RenderTargetView* rtvs[] = {
        mGBuffer.rtv[0].Get(),  // diffuse
        mGBuffer.rtv[1].Get(),  // specular
        mGBuffer.rtv[2].Get(),  // normals
    };

    m_deviceContext->OMSetRenderTargets(kGBufferTexturesCount, rtvs, mDSV.Get());
    // See ClearBuffer
    m_deviceContext->ClearRenderTargetView(mGBuffer.rtv[0].Get(), Colors::LightSteelBlue);
    m_deviceContext->ClearRenderTargetView(mGBuffer.rtv[1].Get(), Colors::Black);
    m_deviceContext->ClearRenderTargetView(mGBuffer.rtv[2].Get(), Colors::Black);
    //m_deviceContext->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);

    m_deviceContext->VSSetShader(m_opaqueVertexShader.Get(), nullptr, 0u);

    m_deviceContext->RSSetViewports(1u, &mViewport);
    m_deviceContext->RSSetState(mRasterizerStateCullBack.Get());

    m_deviceContext->PSSetShader(m_opaquePixelShader.Get(), nullptr, 0u);
    m_deviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());
}

void DeferredRenderer::BindLightingPass()
{
    m_deviceContext->IASetInputLayout(m_lightingVertexShader.GetInputLayout());

    m_deviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), mDSV.Get());
    m_deviceContext->ClearRenderTargetView(mRTV.Get(), Colors::Black);  // to make separate pass in RenderDoc

    m_deviceContext->VSSetShader(m_lightingVertexShader.Get(), nullptr, 0u);

    m_deviceContext->RSSetViewports(1u, &mViewport);
    m_deviceContext->RSSetState(mRasterizerStateCullBack.Get());

    float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    m_deviceContext->OMSetBlendState(mAdditiveBlendState.Get(), nullptr, 0xFFFFFFFF);

    m_deviceContext->PSSetShader(m_lightingPixelShader.Get(), nullptr, 0u);
    // Bind GBuffer resources
    m_deviceContext->PSSetShaderResources(0u, 1u, mGBuffer.srv[0].GetAddressOf());
    m_deviceContext->PSSetShaderResources(1u, 1u, mGBuffer.srv[1].GetAddressOf());
    m_deviceContext->PSSetShaderResources(2u, 1u, mGBuffer.srv[2].GetAddressOf());

    m_deviceContext->PSSetSamplers(0u, 1u, mShadowSamplerState.GetAddressOf());
}

void DeferredRenderer::BindTransparentPass()
{
    m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), nullptr);
    ClearBuffer(.0f);

    m_deviceContext->RSSetViewports(1u, &mViewport);
    m_deviceContext->RSSetState(mRasterizerStateCullBack.Get());

    m_deviceContext->ClearState();
}

void DeferredRenderer::BindParticlesPass()
{
    m_deviceContext->RSSetState(mRasterizerStateCullNone.Get());
    m_deviceContext->RSSetViewports(1u, &mViewport);
    m_deviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), mDSV.Get());
    m_deviceContext->OMSetDepthStencilState(mDSSLessEqual.Get(), 0u);
}

void DeferredRenderer::DrawScreenQuad()
{
    m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);  // Quad -> two triangles
    m_deviceContext->Draw(4u, 0u);
}

void DeferredRenderer::DrawGBuffer()
{
    m_deviceContext->IASetInputLayout(mGBufferVS.GetInputLayout());
    m_deviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), mDSV.Get());

    m_deviceContext->VSSetShader(mGBufferVS.Get(), nullptr, 0u);

    D3D11_VIEWPORT GBufferViewport = 
    {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = static_cast<float>(mScreenWidth * 0.2f),
        .Height = static_cast<float>(mScreenHeight * 0.2f),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f
    };

    m_deviceContext->RSSetViewports(1u, &GBufferViewport);
    m_deviceContext->RSSetState(mRasterizerStateCullBack.Get());

    m_deviceContext->PSSetShader(mGBufferPS.Get(), nullptr, 0u);

    m_deviceContext->PSSetShaderResources(0u, 1u, mGBuffer.srv[m_GBufferLayer % kGBufferTexturesCount].GetAddressOf());
    m_deviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());

    DrawScreenQuad();
}

void DeferredRenderer::BindWithinFrustum()
{
    m_deviceContext->RSSetState(mRasterizerStateCullFront.Get());
    m_deviceContext->OMSetDepthStencilState(mDSSGreater.Get(), 0u);
}

void DeferredRenderer::BindIntersectsFarPlane()
{
    m_deviceContext->RSSetState(mRasterizerStateCullBack.Get());
    m_deviceContext->OMSetDepthStencilState(mDSSLessEqual.Get(), 0u);
}

void DeferredRenderer::BindOutsideFrustum()
{
    m_deviceContext->OMSetDepthStencilState(mDSSLessEqual.Get(), 0u);
}