#pragma once

#include "Renderer.h"
#include <memory>

namespace Scald
{
    constexpr UINT BUFFER_COUNT = 3u;

    class Mesh;

    struct GBuffer
    {
        ID3D11Texture2D* texture[BUFFER_COUNT];
        ID3D11RenderTargetView* rtv[BUFFER_COUNT];
        ID3D11ShaderResourceView* srv[BUFFER_COUNT];
    };

    class DeferredRenderer final : public Renderer
    {
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
        FORCEINLINE void ChangeGBufferLayer(int layer) { GBufferLayer = layer; }

        void BindWithinFrustum();
        void BindIntersectsFarPlane();
        void BindOutsideFrustum();

    private:
        // Deferred Renderer specific
        VertexShader mOpaqueVertexShader;
        PixelShader mOpaquePixelShader;
        VertexShader mLightingVertexShader;
        PixelShader mLightingPixelShader;

        // deferred additional task
        VertexShader mGBufferVS;
        PixelShader mGBufferPS;

        GBuffer mGBuffer;

        std::unique_ptr<Mesh> screenQuad = nullptr;

        // deferred additional task
        std::unique_ptr<Mesh> GBufferTexture = nullptr;
        int GBufferLayer = 0;

        D3D11_VIEWPORT mGBufferViewport = {};
    };
}