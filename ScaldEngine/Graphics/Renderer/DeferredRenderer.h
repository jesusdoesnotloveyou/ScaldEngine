#pragma once

#include "Renderer.h"
#include <memory>

namespace Scald
{
    using Microsoft::WRL::ComPtr;

    const UINT kGBufferTexturesCount = 3u;

    class Mesh;

    struct GBuffer
    {
        ComPtr<ID3D11Texture2D> texture[kGBufferTexturesCount];
        ComPtr<ID3D11RenderTargetView> rtv[kGBufferTexturesCount];
        ComPtr<ID3D11ShaderResourceView> srv[kGBufferTexturesCount];
    };

    class DeferredRenderer final : public Renderer
    {
        using Super = Renderer;
    public:
        DeferredRenderer(IDXGISwapChain1* swapChain, ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, UINT width, UINT height);
        virtual ~DeferredRenderer() noexcept override;

        // Begin of Renderer interface
        virtual void SetupShaders() override;
        virtual void CreateRasterizerState() override;
        // End of Renderer interface

    public:
        void BindGeometryPass();
        void BindLightingPass();
        void BindTransparentPass();
        void BindParticlesPass();

        void DrawScreenQuad();
        // deferred additional task
        void DrawGBuffer();
        FORCEINLINE void ChangeGBufferLayer(int layer) { m_GBufferLayer = layer; }

        void BindWithinFrustum();
        void BindIntersectsFarPlane();
        void BindOutsideFrustum();

    private:
        // Deferred Renderer specific
        VertexShader m_opaqueVertexShader;
        PixelShader m_opaquePixelShader;
        VertexShader m_lightingVertexShader;
        PixelShader m_lightingPixelShader;

        PixelShader m_PBRUberPixelShader;

        // deferred additional task
        VertexShader mGBufferVS;
        PixelShader mGBufferPS;

        GBuffer mGBuffer;
        int m_GBufferLayer = 0;
    };
}