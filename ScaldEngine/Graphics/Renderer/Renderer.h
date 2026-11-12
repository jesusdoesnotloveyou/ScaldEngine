#pragma once

#include "Graphics/DXHelper.h"
#include "Graphics/Shaders.h"

namespace Scald
{
    using Microsoft::WRL::ComPtr;

    class Renderer
    {
    public:
        Renderer(IDXGISwapChain1* spawChain, ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, int width, int height);
        virtual ~Renderer() noexcept = default;

        virtual void SetupShaders();

        virtual void CreateDepthStencilState();
        virtual void CreateRasterizerState();
        virtual void CreateSamplerState();
        virtual void CreateBlendState();

        void ClearBuffer(float r);
        void BindDepthOnlyPass();
    protected:
        // Graphics context. Graphics object manages these resources.
        ID3D11Device1* m_device = nullptr;
        ID3D11DeviceContext1* m_deviceContext = nullptr;

        int mScreenHeight;
        int mScreenWidth;

        // Render Target
        ComPtr<ID3D11RenderTargetView> mRTV;
        // Depth Stencil
        ComPtr<ID3D11DepthStencilView> mDSV;

        // TODO: should probably be moved to DeferredRenderer
        ComPtr<ID3D11DepthStencilState> mDSSLessEqual;
        ComPtr<ID3D11DepthStencilState> mDSSGreater;
        // Rast
        ComPtr<ID3D11RasterizerState> mRasterizerStateCullBack;
        ComPtr<ID3D11RasterizerState> mRasterizerStateCullFront;
        ComPtr<ID3D11RasterizerState> mRasterizerStateCullNone;
        // Sampler
        ComPtr<ID3D11SamplerState> mSamplerState;
        ComPtr<ID3D11SamplerState> mShadowSamplerState;
        // Blend
        ComPtr<ID3D11BlendState> mAdditiveBlendState;

        D3D11_VIEWPORT mViewport = {};

    protected:
        VertexShader mShadowVertexShader;
        GeometryShader mCSMGeometryShader;
    };
}