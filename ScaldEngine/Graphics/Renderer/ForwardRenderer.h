#pragma once

#include "Renderer.h"

namespace Scald
{
    class ForwardRenderer final : public Renderer
    {
        using Super = Renderer;
    public:
        ForwardRenderer(IDXGISwapChain1* swapChain, ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, UINT width, UINT height);
        virtual ~ForwardRenderer() noexcept override = default;

        // Begin of Renderer interface
        virtual void SetupShaders() override;

    private:
        // Forward rendering shaders
        VertexShader mVertexShader;
        PixelShader mPixelShader;
    };
}