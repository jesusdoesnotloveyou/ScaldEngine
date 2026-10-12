#include "stdafx.h"
#include "DeferredRenderer.h"
#include "Graphics/ScaldCoreTypes.h"
#include "Graphics/Mesh.h"
#include "ScaldException.h"

using namespace Scald;

namespace
{
    const wchar_t* kOpaqueVertexShaderPath           = L"./Data/Shaders/DeferredOpaqueVS.hlsl";
    const wchar_t* kOpaquePixelShaderPath            = L"./Data/Shaders/DeferredOpaquePS.hlsl";
    const wchar_t* kDeferredLightingVertexShaderPath = L"./Data/Shaders/DeferredLightingVS.hlsl";
    const wchar_t* kDeferredLightingPixelShaderPath  = L"./Data/Shaders/DeferredLightingPS.hlsl";
    const wchar_t* kDrawGBufferVertexShaderPath      = L"./Data/Shaders/GBufferVS.hlsl";
    const wchar_t* kDrawGBufferPixelShaderPath       = L"./Data/Shaders/GBufferPS.hlsl";
}  // namespace

DeferredRenderer::DeferredRenderer(IDXGISwapChain* swapChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
    : Super(swapChain, device, deviceContext, width, height)
{
    // See Step 03 in Graphics -> same logic, but in this case there is a number of rtvs
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
        ThrowIfFailed(device->CreateRenderTargetView(mGBuffer.texture[i], &renderTargetViewDesc, &mGBuffer.rtv[i]));
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    ZeroMemory(&textureDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1u;

    for (UINT i = 0; i < kGBufferTexturesCount; i++)
    {
        ThrowIfFailed(device->CreateShaderResourceView(mGBuffer.texture[i], &shaderResourceViewDesc, &mGBuffer.srv[i]));
    }

    std::vector<VertexPositionNormalUV> quadVertices = {VertexPositionNormalUV(), VertexPositionNormalUV(), VertexPositionNormalUV(), VertexPositionNormalUV()};
    std::vector<DWORD> quadIndeces = {0};  // at least one due to throwing exception in Init
    m_screenQuad = std::make_unique<Mesh>(device, quadVertices, quadIndeces);
    m_GBufferQuad = std::make_unique<Mesh>(device, quadVertices, quadIndeces);
}

DeferredRenderer::~DeferredRenderer() noexcept
{
    for (UINT i = 0; i < kGBufferTexturesCount; i++)
    {
        mGBuffer.texture[i]->Release();
        mGBuffer.rtv[i]->Release();
        mGBuffer.srv[i]->Release();
    }
}

void DeferredRenderer::SetupShaders()
{
    Super::SetupShaders();

    D3D11_INPUT_ELEMENT_DESC inputLayoutOpaqueDesc[] = {
        D3D11_INPUT_ELEMENT_DESC{"POSITION", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        D3D11_INPUT_ELEMENT_DESC{"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        D3D11_INPUT_ELEMENT_DESC{"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u}};

    D3D11_INPUT_ELEMENT_DESC inputLayoutLightingDesc[] = {
        D3D11_INPUT_ELEMENT_DESC{"POSITION", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        D3D11_INPUT_ELEMENT_DESC{"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u}};

    D3D11_INPUT_ELEMENT_DESC inputLayoutGBufferDesc[] = {
        D3D11_INPUT_ELEMENT_DESC{"POSITION", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
        D3D11_INPUT_ELEMENT_DESC{"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u}};

    mGBufferVS.Init(m_device, inputLayoutGBufferDesc, (UINT)std::size(inputLayoutGBufferDesc), kDrawGBufferVertexShaderPath);
    mGBufferPS.Init(m_device, kDrawGBufferPixelShaderPath);

    m_opaqueVertexShader.Init(m_device, inputLayoutOpaqueDesc, (UINT)std::size(inputLayoutOpaqueDesc), kOpaqueVertexShaderPath);
    m_opaquePixelShader.Init(m_device, kOpaquePixelShaderPath);

    m_lightingVertexShader.Init(m_device, inputLayoutLightingDesc, (UINT)std::size(inputLayoutLightingDesc), kDeferredLightingVertexShaderPath);
    m_lightingPixelShader.Init(m_device, kDeferredLightingPixelShaderPath);
}

void DeferredRenderer::CreateRasterizerState()
{
    // Step 10: Setup Rasterizer Stage and Viewport
    Super::CreateRasterizerState();

    m_GBufferViewport.TopLeftX = 0.0f;
    m_GBufferViewport.TopLeftY = 0.0f;
    m_GBufferViewport.Width = static_cast<float>(mScreenWidth * 0.2f);
    m_GBufferViewport.Height = static_cast<float>(mScreenHeight * 0.2f);
    m_GBufferViewport.MinDepth = 0.0f;
    m_GBufferViewport.MaxDepth = 1.0f;

}

void DeferredRenderer::BindGeometryPass()
{
    m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->IASetInputLayout(m_opaqueVertexShader.GetInputLayout());

    ID3D11RenderTargetView* rtvs[] = {
        mGBuffer.rtv[0],  // diffuse
        mGBuffer.rtv[1],  // specular
        mGBuffer.rtv[2],  // normals
    };

    // See Step 11
    m_deviceContext->OMSetRenderTargets(kGBufferTexturesCount, rtvs, mDSV.Get());
    // See ClearBuffer
    m_deviceContext->ClearRenderTargetView(mGBuffer.rtv[0], Colors::LightSteelBlue);
    m_deviceContext->ClearRenderTargetView(mGBuffer.rtv[1], Colors::Black);
    m_deviceContext->ClearRenderTargetView(mGBuffer.rtv[2], Colors::Black);
    // mDeviceContext->ClearDepthStencilView(mDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);

    m_deviceContext->VSSetShader(m_opaqueVertexShader.Get(), nullptr, 0u);

    m_deviceContext->RSSetViewports(1u, &mViewport);
    m_deviceContext->RSSetState(mRasterizerStateCullBack.Get());

    // Step 09: Set Pixel Shaders
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
    m_deviceContext->PSSetShaderResources(0u, 1u, &mGBuffer.srv[0]);
    m_deviceContext->PSSetShaderResources(1u, 1u, &mGBuffer.srv[1]);
    m_deviceContext->PSSetShaderResources(2u, 1u, &mGBuffer.srv[2]);

    m_deviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());
    m_deviceContext->PSSetSamplers(1u, 1u, mShadowSamplerState.GetAddressOf());
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
    auto& quadVB = m_screenQuad->GetVertexBuffer();
    m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);  // Quad -> two triangles
    m_deviceContext->IASetVertexBuffers(0u, 1u, quadVB.GetAddressOf(), quadVB.GetStridePtr(), quadVB.GetOffsetPtr());
    m_deviceContext->IASetIndexBuffer(m_screenQuad->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
    m_deviceContext->Draw(quadVB.GetBufferSize(), 0u);
}

void DeferredRenderer::DrawGBuffer()
{
    m_deviceContext->IASetInputLayout(mGBufferVS.GetInputLayout());
    m_deviceContext->OMSetRenderTargets(1u, mRTV.GetAddressOf(), mDSV.Get());

    m_deviceContext->VSSetShader(mGBufferVS.Get(), nullptr, 0u);

    m_deviceContext->RSSetViewports(1u, &m_GBufferViewport);
    m_deviceContext->RSSetState(mRasterizerStateCullBack.Get());

    m_deviceContext->PSSetShader(mGBufferPS.Get(), nullptr, 0u);

    m_deviceContext->PSSetShaderResources(0u, 1u, &mGBuffer.srv[m_GBufferLayer % kGBufferTexturesCount]);
    m_deviceContext->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());

    auto& GBufferVB = m_GBufferQuad->GetVertexBuffer();
    m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);  // Quad -> two triangles
    m_deviceContext->IASetVertexBuffers(0u, 1u, GBufferVB.GetAddressOf(), GBufferVB.GetStridePtr(), GBufferVB.GetOffsetPtr());
    m_deviceContext->IASetIndexBuffer(m_GBufferQuad->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0u);
    m_deviceContext->Draw(GBufferVB.GetBufferSize(), 0u);
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