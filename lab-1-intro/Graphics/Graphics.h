#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d.h>

#include <wrl.h>
#include <vector>

#include "Shaders.h"
#include "ConstantBuffer.h"
#include "Light/LightHelper.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

class SceneGeometry;
class Light;
class Camera;
class ThirdPersonCamera;

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics();

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	void Setup();
	void InitSceneObjects(std::vector<SceneGeometry*>& sceneObjects);
	void AddLightSourceParams(PointLight* lightParams);
	void UpdateLightParams(SceneGeometry* light);

	void ClearBuffer(float r);
	void DrawScene(std::vector<SceneGeometry*>& sceneObjects);
	void EndFrame();

	FORCEINLINE ThirdPersonCamera* GetCamera() const { return mTPCamera; }
private:
	void SetupShaders();
	void SetupLight();

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
	Camera* mCamera = nullptr;
	ThirdPersonCamera* mTPCamera = nullptr;

	int mScreenWidth;
	int mScreenHeight;
	HWND hWnd;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

#pragma region Light
	ConstantBuffer<ConstBufferPerFrame> mCB;

	// need to update members of vector
	std::vector<PointLight> mLightsParameters;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mLightShaderResourceView;
#pragma endregion Light

	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRtv;

	// Depth Stencil
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDsv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

	// Rast
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;

	// Sampler
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
};