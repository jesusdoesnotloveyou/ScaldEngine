#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d.h>

#include <wrl.h>
#include <vector>

#include "Shaders.h"
#include "ConstantBuffer.h"
#include "ScaldCoreTypes.h"

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

	void ClearBuffer(float r);
	void DrawScene(std::vector<SceneGeometry*>& sceneObjects);
	void EndFrame();

	FORCEINLINE ThirdPersonCamera* GetCamera() const { return mTPCamera; }
private:
	void SetupShaders();

private:
	Camera* mCamera = nullptr;
	ThirdPersonCamera* mTPCamera = nullptr;
	std::vector<Light*> mLights;

	int mScreenWidth;
	int mScreenHeight;
	HWND hWnd;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

	ConstantBuffer<ConstBufferPerFrame> mCB;

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