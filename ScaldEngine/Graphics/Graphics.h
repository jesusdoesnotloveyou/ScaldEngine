#pragma once

#include "ScaldCoreTypes.h"
#include "LightHelper.h"
#include "Shaders.h"
#include "ConstantBuffer.h"
#include "StructuredBuffer.h"

#include <array>
#include <memory>
#include <cstdint>

namespace Scald
{
    using namespace Microsoft::WRL;
    using namespace DirectX;

    class Camera;
    class CascadeShadowMap;
    class DeferredRenderer;
    class FireParticleSystem;
    class Light;
    class PrimitiveComponent; // Renderable scene components
    class Scene;              // Scene contains all renderables (lights, meshes)
    class Model;

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
        void CreateDepthStencilState();
        void CreateRasterizerState();
        void CreateSamplerState();
        void CreateBlendState();

        void SetupShaders();

        void BindGeometryPassResources();
        void BindLightingPassResources();

        void RenderDepthOnlyPass(Scene* scene);
        void RenderGeometry(Scene* scene);
        void RenderLighting(Scene* scene);
        void RenderParticles();

        void RenderDirectionalLight(Scene* scene);
        void RenderOmniLight(Scene* scene);
        void RenderSpotLight(Scene* scene);

        // @todo: From Luna's book
        void UpdateObjectCB(const XMMATRIX& world/*, float deltaTime*/);
        void UpdatePerFrameCB(/*float deltaTime*/);
        // deferred additional task specific
        void RenderGBuffer();

        void DrawModel(const Model* modelToDraw);
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
        std::unique_ptr<Camera> m_camera = nullptr;
        
        uint32_t m_screenWidth;
        uint32_t m_screenHeight;
        // should encapsulate in camera
        float mCameraFarZ = 500.0f;
        float mCameraNearZ = 0.1f;
        float mFovDegrees = 90.0f;

        bool m_bIsPointLightEnabled = true;
        bool m_bIsDirectionalLightEnabled = true;
        bool m_bIsSpotLightEnabled = true;
        bool m_bIsDeferredRenderingApplied = true;

        VertexShader mShadowVertexShader;
        VertexShader mVertexShader;
        PixelShader mPixelShader;
        GeometryShader mCSMGeometryShader;

    #pragma region Light
        // like constant buffer per object, but for lights
        // could be implemented due to encapsulation inside light class
        // TODO: structured buffer instead
        ConstantBuffer<ConstantBufferPerObject> mCB_LightVolume;
        ConstantBuffer<LIGHT_DESC> mCB_Light;
    #pragma endregion Light

        ConstantBuffer<ConstantBufferPerObject> m_perObjectCB;
        ConstantBuffer<ConstantBufferPerFrame> m_perFrameCB;

        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_deviceContext;

        std::unique_ptr<DeferredRenderer> pRenderer;
        std::unique_ptr<FireParticleSystem> pFireParticleSystem;

        // Shadows
        // TODO: should probably placed in light class
        std::unique_ptr<CascadeShadowMap> mCascadeShadowMap = nullptr;
        ConstantBuffer<ConstantBufferCascadeShadows> mCB_CSM;
    };
}