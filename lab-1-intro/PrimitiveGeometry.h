#pragma once

#include "ScaldCoreTypes.h"
#include "RenderComponent.h"
#include <cmath>
#include <vector>

class RenderComponent;

class PrimitiveGeometry
{
public:
	PrimitiveGeometry();
	PrimitiveGeometry(const std::vector<Vertex>& v, const std::vector<int>& i);
	/*PrimitiveGeometry(const PrimitiveGeometry&) = delete;
	PrimitiveGeometry(PrimitiveGeometry&&);
	PrimitiveGeometry& operator=(const PrimitiveGeometry&) = delete;
	PrimitiveGeometry operator=(PrimitiveGeometry&&);*/
	virtual ~PrimitiveGeometry();

	void Initialize(ID3D11Device* pDevice);

	ID3D11Buffer* const* GetAddressOfVertexBuffer() const;
	ID3D11Buffer* const* GetAddressOfIndexBuffer() const;
	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;

	RenderComponent* GetRenderComponent() const;

	UINT stride = { sizeof(Vertex) };
	UINT offset = { 0 };

protected:

private:
	std::vector<Vertex> vertices = {
		{ DirectX::XMFLOAT4(0.2f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.2f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.2f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.2f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};
	std::vector<int> indeces = { 0, 1, 2,  1, 0, 3 };

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};