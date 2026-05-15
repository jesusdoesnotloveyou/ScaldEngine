#pragma once

#include "ScaldCore/Engine/ScaldTimer.h"
#include "Shaders.h"
#include "ConstantBuffer.h"
#include "ScaldCoreTypes.h"
#include "Games/Katamari/KatamariPlayer.h"

class SceneGeometry;
class Light;
class DirectionalLight;
class PointLight;
class SpotLight;
class Camera;
class ThirdPersonCamera;
class CascadeShadowMap;
class DeferredRenderer;
class FireParticleSystem;

class Graphics
{
public:
    Graphics(HWND hWnd, int width, int height);
    ~Graphics();

    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    void Setup();

    void AddPlayer(std::shared_ptr<KatamariPlayer> player) { mPlayer = player; }

    template <typename T>
    void AddToRenderPool(std::shared_ptr<T> sceneObject)
    {
        static_assert(std::is_base_of<SceneComponent, T>::value, "Render object must be a scene component!");

        const auto& lightObject = std::dynamic_pointer_cast<Light>(sceneObject);
        if (lightObject)
        {
            if (lightObject->GetLightType() == ELightType::Directional)
            {
                mDirectionalLight = lightObject;
            }
            else if (lightObject->GetLightType() == ELightType::Point)
            {
                mLights.push_back(lightObject);
            }
            // else if (Spot)
            //  ...
        }

        mRenderObjects.emplace_back(std::move(sceneObject));
    }

    void InitSceneObjects();

    void ClearBuffer(float r);
    void DrawScene(const ScaldTimer& st);
    void EndFrame();

    void Update(const ScaldTimer& st);
    FORCEINLINE ThirdPersonCamera* GetCamera() const { return mTPCamera.get(); }

private:
    void CreateDepthStencilState();
    void CreateRasterizerState();
    void CreateSamplerState();
    void CreateBlendState();

    void SetupShaders();

    void BindGeometryPassResources();
    void BindLightingPassResources();

    void RenderDepthOnlyPass();
    void RenderLighting();
    void RenderParticles(float deltaTime);

    void RenderDirectionalLight();
    void RenderOmniLights();
    void RenderSpotLights();

    // deferred additional task specific
    void RenderGBuffer();

public:
    void SwitchGBufferLayer(int layer);

private:
    void UpdateDirLightConstantBuffer(Light* dirLight);
    void UpdateOmniLightConstantBuffer(Light* omniLight);
    void UpdateSpotLightConstantBuffer(Light* spotLight);

    // get all 8 vertices of frustrum
    std::vector<XMVECTOR> GetFrustumCornersWorldSpace(const XMMATRIX& viewProjection);
    XMMATRIX GetLightSpaceMatrix(const float nearPlane, const float farPlane);
    void GetLightSpaceMatrices(std::vector<XMMATRIX>& outMatrices);

    template <typename T>
    bool ApplyChanges(ID3D11DeviceContext* deviceContext, ID3D11Buffer* buffer, const std::vector<T>& bufferData)
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ThrowIfFailed(deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

        CopyMemory(mappedResource.pData, bufferData.data(), sizeof(T) * bufferData.size());
        deviceContext->Unmap(buffer, 0);
        return true;
    }

    template <typename T>
    HRESULT CreateStructuredBuffer(ID3D11Device* device, ID3D11Buffer** buffer, const std::vector<T>& bufferData)
    {
        UINT stride = (UINT)sizeof(T);
        UINT byteWidth = stride * (UINT)bufferData.size();

        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = byteWidth;
        desc.Usage = D3D11_USAGE_DYNAMIC;              // to use map/unmap to update
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  // from cpu
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;   // to get from GPU
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.StructureByteStride = stride;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = bufferData.data();
        data.SysMemPitch = 0u;
        data.SysMemSlicePitch = 0u;

        return device->CreateBuffer(&desc, &data, buffer);
    }

private:
    HWND hWnd;
    int mScreenWidth;
    int mScreenHeight;

public:
    std::shared_ptr<KatamariPlayer> mPlayer;
    std::vector<std::shared_ptr<SceneGeometry>> mRenderObjects;

    std::vector<std::shared_ptr<Light>> mLights;         // deferred rendering stuff
    std::shared_ptr<Light> mDirectionalLight = nullptr;  // as well as this
private:
    std::unique_ptr<ThirdPersonCamera> mTPCamera = nullptr;
    // should encapsulate in camera
    float mCameraFarZ = 500.0f;
    float mCameraNearZ = 0.1f;
    float mFovDegrees = 90.0f;

    bool bIsPointLightEnabled = true;
    bool bIsDirectionalLightEnabled = true;
    bool bIsSpotLightEnabled = true;

    bool bIsDeferredRenderingTechniqueApplied = true;

    VertexShader mShadowVertexShader;
    VertexShader mVertexShader;
    PixelShader mPixelShader;
    GeometryShader mCSMGeometryShader;

#pragma region Light
    // like constant buffer per object, but for lights
    // could be implemented due to encapsulation inside light class
    ConstantBuffer<ConstantBufferPerObject> mCB_LightVolume;
    ConstantBufferPerObject mLightVolumeData;

    ConstantBuffer<ConstantBufferPerFrame> mCB_PerFrame;
    ConstantBufferPerFrame mPerFrameData;
#pragma endregion Light

    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;

    // Renderer
    // Deferred Rendering
    std::unique_ptr<DeferredRenderer> pRenderer;
    // Particles
    std::unique_ptr<FireParticleSystem> pFireParticleSystem;

#pragma region DeferredLightManagement
    ConstantBuffer<LIGHT_DESC> mCB_Light;
    LIGHT_DESC mLightData;
#pragma endregion DeferredLightManagement

    // Shadows
    // TODO: should probably placed in light class
    std::unique_ptr<CascadeShadowMap> mCascadeShadowMap = nullptr;
    ConstantBuffer<CascadeDataConstantBuffer> mCB_CSM;
    CascadeDataConstantBuffer mCSMData;
};