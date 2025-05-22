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
class PointLight;
class DirectionalLight;
class SpotLight;
class Camera;
class ThirdPersonCamera;
class CascadeShadowMap;
class DeferredRenderer;
class ForwardRenderer;

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

#pragma region LightManagment
	void AddPointLightSourceParams(PointLightParams* lightParams);
	void UpdatePointLightParams();

	void AddDirectionalLightSourceParams(DirectionalLightParams* lightParams);
	void UpdateDirectionalLightParams();

	void AddSpotLightSourceParams(SpotLightParams* lightParams);
	void UpdateSpotLightParams();
#pragma endregion LightManagment

	void ClearBuffer(float r);
	void DrawScene();
	void EndFrame();

	void Update(const ScaldTimer& st);
	FORCEINLINE ThirdPersonCamera* GetCamera() const { return mTPCamera; }
private:
	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateSamplerState();

	void SetupShaders();
	void InitPointLight();
	void InitDirectionalLight();
	void RenderDepthOnlyPass();
	void RenderColorPass();

	// get all 8 vertices of frustrum
	std::vector<XMVECTOR> GetFrustumCornersWorldSpace(const XMMATRIX& viewProjection);
	XMMATRIX GetLightSpaceMatrix(const float nearPlane, const float farPlane);
	void GetLightSpaceMatrices(std::vector<XMMATRIX>& outMatrices);
	void UpdateShadowCascadeSplits();

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
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = bufferData.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		return device->CreateBuffer(&desc, &data, buffer);
	}

private:
	int mScreenWidth;
	int mScreenHeight;
	HWND hWnd;

public:
	std::vector<SceneGeometry*> mRenderObjects;
private:
	// temporary, need a LightManager that would control light pool
	std::vector<PointLight*> mPointLights;
	std::vector<DirectionalLight*> mDirectionalLights;
	std::vector<SpotLight*> mSpotLights;

	bool bIsPointLightEnabled = false;
	bool bIsDirectionalLightEnabled = true;
	bool bIsSpotLightEnabled = false;

	Camera* mCamera = nullptr;
	ThirdPersonCamera* mTPCamera = nullptr;
	float mCameraFarZ = 500.0f;
	float mCameraNearZ = 0.1f;
	float mFovDegrees = 90.0f;
	
	VertexShader mShadowVertexShader;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	GeometryShader mCSMGeometryShader;

#pragma region Light
	ConstantBuffer<ConstBufferVSPerFrame> mCBVSPerFrame;
	ConstantBuffer<ConstBufferPSPerFrame> mCBPSPerFrame;
	ConstantBuffer<CascadeData> mCB_CSM;

	// need to update members of vector
	std::vector<PointLightParams> mPointLightsParameters;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mPointLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mPointLightShaderResourceView; // structured buffer

	std::vector<DirectionalLightParams> mDirectionalLightParameters;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mDirectionalLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mDirectionalLightShaderResourceView; // structured buffer
#pragma endregion Light

	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRTV;
	// Depth Stencil
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDSV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
	// Rast
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
	// Sampler
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mShadowSamplerState;

	D3D11_VIEWPORT currentViewport = {};

	// Shadows
	CascadeShadowMap* mCascadeShadowMap = nullptr;
	float cascadeSplitLambda = 0.95f; // idk
	float shadowCascadeLevels[CASCADE_NUMBER] = { 0.0f, 0.0f, 0.0f, 0.0f };
	CascadeData mCSMData;

	// Renderer
	// Deferred Rendering
	std::unique_ptr<DeferredRenderer> pRenderer;
};