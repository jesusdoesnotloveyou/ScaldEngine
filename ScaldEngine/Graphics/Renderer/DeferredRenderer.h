#pragma once

#include "Renderer.h"
#include <memory>

namespace Scald
{
    const UINT kGBufferTexturesCount = 3u;

    class Mesh;

    struct GBuffer
    {
        ID3D11Texture2D* texture[kGBufferTexturesCount];
        ID3D11RenderTargetView* rtv[kGBufferTexturesCount];
        ID3D11ShaderResourceView* srv[kGBufferTexturesCount];
    };

    class DeferredRenderer final : public Renderer
    {
        using Super = Renderer;
    public:
        DeferredRenderer(IDXGISwapChain* swapChain, ID3D11Device* device, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
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

        // deferred additional task
        VertexShader mGBufferVS;
        PixelShader mGBufferPS;

        GBuffer mGBuffer;

        std::unique_ptr<Mesh> m_screenQuad = nullptr;

        // deferred additional task
        std::unique_ptr<Mesh> m_GBufferQuad = nullptr;
        int m_GBufferLayer = 0;

        D3D11_VIEWPORT m_GBufferViewport = {};
    };
}