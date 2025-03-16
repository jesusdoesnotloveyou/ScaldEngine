#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d.h>
#include <wrl.h>
#include <vector>

#include "Camera.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

class PrimitiveGeometry;

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics() = default;

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	void Setup();

	void ClearBuffer(float r);
	void DrawScene(std::vector<PrimitiveGeometry*>& gameObjects);
	void EndFrame();

	ID3D11DeviceContext* GetDeviceContext() const;
	ID3D11Device* GetDevice() const;

private:
	int mScreenWidth;
	int mScreenHeight;
	HWND hWnd;

	Camera mCamera;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRtv;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRastState;
};