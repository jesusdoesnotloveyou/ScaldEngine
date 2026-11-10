#pragma once

#include "ScaldCoreTypes.h"
#include "LightHelper.h"
#include "ConstantBuffer.h"
// For light and particles
#include "StructuredBuffer.h"

#include <array>
#include <memory>
#include <cstdint>

namespace Scald
{
    using Microsoft::WRL::ComPtr;

    class Camera;
    class CascadeShadowMap;
    class DeferredRenderer;
    class FireParticleSystem;
    class Light;
    class PrimitiveComponent; // Renderable scene components
    class Scene;              // Scene contains all renderables (lights, meshes)
    class Model;
    struct Texture;

    class Graphics
    {
    public:
        Graphics(HWND hWnd, int width, int height);
        ~Graphics();

        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;

        void Setup();

        void ClearBuffer(float r);
        void DrawScene(Scene* scene);
        void EndFrame();

        void Update(float deltaTime);
    private:
        void BindPerFrameResources();
        void BindLightingPassResources();

        void RenderDepthOnlyPass(Scene* scene);
        void RenderGeometry(Scene* scene);
        void RenderLighting(Scene* scene);
        void RenderParticles();

        void RenderDirectionalLight(Scene* scene);
        void RenderOmniLight(Scene* scene);
        void RenderSpotLight(Scene* scene);

        /*
         * This sets all input/output resource slots, shaders, input layouts, predications, scissor rectangles,
         * depth-stencil state, rasterizer state, blend state, sampler state, and viewports to NULL.
         * The primitive topology is set to UNDEFINED.
         */
        void ClearContext();

        // From Luna's book
        void UpdateObjectCB(const XMMATRIX& world/*, float deltaTime*/);
        void UpdatePerFrameCB(/*float deltaTime*/);
        // deferred additional task specific
        void RenderGBuffer();

        void DrawModel(const Model* modelToDraw);
        void DrawTexture(const Texture* textureView);
    public:
        void SwitchGBufferLayer(int layer);

        template <typename T = Camera>
        T* GetActiveCamera() const
        {
            // Could be unsafe when downcasting: dynamic should be instead
            return static_cast<T*>(m_camera.get());
        }

        ID3D11Device* GetDevice() const
        { 
            return m_device ? m_device.Get() : nullptr;
        }

    private:
        // Update structured buffer
        void UpdateDirectionalLights();
        // Update structured buffer
        void UpdateOmniLightParams();
        // Update structured buffer
        void UpdateSpotLightParams();

        // get all 8 vertices of frustrum
        std::array<XMVECTOR, 8u> GetFrustumCornersWorldSpace(const XMMATRIX& viewProjection);
        XMMATRIX GetLightSpaceMatrix(const float nearPlane, const float farPlane);
        void GetLightSpaceMatrices(std::array<XMMATRIX, kCascadeNumber>& outMatrices);

    private:
        std::unique_ptr<Camera> m_camera;
        
        D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_UNKNOWN;
        D3D_FEATURE_LEVEL m_featureLevel;

        uint32_t m_screenWidth;
        uint32_t m_screenHeight;
        // should encapsulate in camera
        float m_cameraFarZ = 500.0f;
        float m_cameraNearZ = 0.1f;
        float m_fovDegrees = 90.0f;

        bool m_bIsPointLightEnabled = true;
        bool m_bIsDirectionalLightEnabled = true;
        bool m_bIsSpotLightEnabled = true;
        bool m_bIsDeferredRenderingApplied = true;
    #pragma region Light
        // like constant buffer per object, but for lights
        // could be implemented due to encapsulation inside light class
        // TODO: structured buffer instead
        ConstantBuffer<PerObjectConstants> m_lightVolumeCB;
        ConstantBuffer<LIGHT_DESC> m_lightCB;
    #pragma endregion Light

        ConstantBuffer<PerObjectConstants> m_perObjectCB;
        ConstantBuffer<PerFrameConstants> m_perFrameCB;

        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<IDXGISwapChain1> m_swapChain1;
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11Device1> m_device1;
        ComPtr<ID3D11DeviceContext> m_immediateContext;
        ComPtr<ID3D11DeviceContext1> m_immediateContext1;

        std::unique_ptr<DeferredRenderer> m_renderer;
        std::unique_ptr<FireParticleSystem> m_fireParticleSystem;

        // Shadows
        // TODO: should probably placed in light class
        std::unique_ptr<CascadeShadowMap> m_cascadeShadowMap;
        ConstantBuffer<CascadeShadowsConstants> m_csmCB;
    };
}