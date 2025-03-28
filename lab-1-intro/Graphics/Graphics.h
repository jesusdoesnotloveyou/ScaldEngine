#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d.h>

#include <wrl.h>
#include <vector>

#include <WICTextureLoader.h>

#include "Camera.h"
#include "Shaders.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

class SceneGeometry;

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics() = default;

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	void Setup();
	void InitSceneObjects(std::vector<SceneGeometry*>& sceneObjects);

	void ClearBuffer(float r);
	void DrawScene(std::vector<SceneGeometry*>& sceneObjects);
	void EndFrame();

	Camera mCamera;
private:
	void SetupShaders();

private:
	int mScreenWidth;
	int mScreenHeight;
	HWND hWnd;

	VertexShader mVertexShader;
	PixelShader mPixelShader;

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
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
};