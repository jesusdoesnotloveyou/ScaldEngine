#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d.h>
#include <wrl.h>
#include <vector>

#include "../Objects/Geometry/PrimitiveGeometry.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics() = default;

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	void Setup();

	void ClearBuffer(float r);
	void Draw(float angle);
	void EndFrame();

	std::vector<PrimitiveGeometry*> GameObjects;

private:
	int screenWidth;
	int screenHeight;
	HWND hWnd;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRtv;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRastState;
};