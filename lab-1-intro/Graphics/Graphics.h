#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d.h>

#include <wrl.h>
#include <vector>
#include <memory>

#include "../ScaldCore/Engine/ScaldTimer.h"
#include "Shaders.h"
#include "ConstantBuffer.h"
#include "ScaldCoreTypes.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

class SceneGeometry;
class Light;
class DirectionalLight;
class PointLight;
class SpotLight;
class Camera;
class ThirdPersonCamera;
class CascadeShadowMap;
class DeferredRenderer;
class ForwardRenderer;
class FireParticleSystem;

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics();

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	void Setup();
	void AddToRenderPool(SceneGeometry* sceneObject);
	void InitSceneObjects();

#pragma region ForwardRenderingLightManagment
	void AddPointLightSourceParams(PointLightParams* lightParams);
	void UpdatePointLightsParams();

	void AddSpotLightSourceParams(SpotLightParams* lightParams);
	void UpdateSpotLightsParams();
#pragma endregion ForwardRenderingLightManagment

	void ClearBuffer(float r);
	void DrawScene();
	void EndFrame();

	void Update(const ScaldTimer& st);
	FORCEINLINE ThirdPersonCamera* GetCamera() const { return mTPCamera; }
private:
	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();

	void SetupShaders();
	
	void InitPointLights();
	void InitSpotLights();

	void BindLightingPassResources();

	void RenderDepthOnlyPass();
	void RenderColorPass();
	void RenderLighting();
	void RenderParticles();

	void UpdateLightConstantBuffer(Light* light);

	// get all 8 vertices of frustrum
	std::vector<XMVECTOR> GetFrustumCornersWorldSpace(const XMMATRIX& viewProjection);
	XMMATRIX GetLightSpaceMatrix(const float nearPlane, const float farPlane);
	void GetLightSpaceMatrices(std::vector<XMMATRIX>& outMatrices);

	// should be moved to point light class
	float CalcPointLightRange(const Light& light);

	template<typename T>
	bool ApplyChanges(ID3D11DeviceContext* deviceContext, ID3D11Buffer* buffer, const std::vector<T>& bufferData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ThrowIfFailed(deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		CopyMemory(mappedResource.pData, bufferData.data(), sizeof(T) * bufferData.size());
		deviceContext->Unmap(buffer, 0);
		return true;
	}

	template<typename T>
	HRESULT CreateStructuredBuffer(ID3D11Device* device, ID3D11Buffer** buffer, const std::vector<T>& bufferData)
	{
		UINT stride = (UINT)sizeof(T);
		UINT byteWidth = stride * (UINT)bufferData.size();

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = byteWidth;
		desc.Usage = D3D11_USAGE_DYNAMIC;				// to use map/unmap to update
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// from cpu
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// to get from GPU
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = bufferData.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		return device->CreateBuffer(&desc, &data, buffer);
	}

private:
	HWND hWnd;
	int mScreenWidth;
	int mScreenHeight;

public:
	std::vector<SceneGeometry*> mRenderObjects;
	std::vector<Light*> mLights; // deferred rendering stuff
	Light* mDirectionalLight = nullptr; // as well as this
private:
	// temporary, need a LightManager that would control light pool
	// forward rendering specific
	std::vector<PointLight*> mPointLights;
	std::vector<SpotLight*> mSpotLights;

	bool bIsPointLightEnabled = true;
	bool bIsDirectionalLightEnabled = true;
	bool bIsSpotLightEnabled = true;

	Camera* mCamera = nullptr;
	ThirdPersonCamera* mTPCamera = nullptr;
	// should encapsulate in camera
	float mCameraFarZ = 500.0f;
	float mCameraNearZ = 0.1f;
	float mFovDegrees = 90.0f;
	
	VertexShader mShadowVertexShader;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	GeometryShader mCSMGeometryShader;

#pragma region Light
	ConstantBuffer<ConstBufferVS> mCB_LightVolume;
	ConstBufferVS mLightVolumeData;

	ConstantBuffer<ConstantBufferPerFrame> mCB_PerFrame;
	ConstantBufferPerFrame mPerFrameData;

	// need to update member
	std::vector<DirectionalLightParams> mDirectionalLightParameters;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mDirectionalLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mDirectionalLightSRV; // structured buffer
	
	// need to update members of vector
	std::vector<PointLightParams> mPointLightsParameters;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPointLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mPointLightSRV; // structured buffer

	// need to update members of vector
	std::vector<SpotLightParams> mSpotLightsParameters;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mSpotLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSpotLightSRV; // structured buffer
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
	CascadeShadowMap* mCascadeShadowMap = nullptr;
	ConstantBuffer<CascadeDataConstantBuffer> mCB_CSM;
	CascadeDataConstantBuffer mCSMData;

	bool bIsForwardRenderingTechniqueApplied = false;
	bool bIsDeferredRenderingTechniqueApplied = true;
	bool bIsForwardPlusRenderingTechniqueApplied = false;
};