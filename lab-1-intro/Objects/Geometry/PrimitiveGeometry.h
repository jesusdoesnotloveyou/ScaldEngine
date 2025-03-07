#pragma once

#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/IndexBuffer.h"
#include "../../Graphics/ConstantBuffer.h"
#include "../../Graphics/ScaldCoreTypes.h"

#include "../../Objects/Components/RenderComponent.h"
#include "../../Objects/Components/CollisionComponent.h"

#include <cmath>
#include <vector>

class RenderComponent;

class PrimitiveGeometry
{
public:
	PrimitiveGeometry();
	PrimitiveGeometry(const std::vector<Vertex>& v, const std::vector<DWORD>& i);
	//PrimitiveGeometry(const PrimitiveGeometry& lhs);
	//PrimitiveGeometry(PrimitiveGeometry&& rhs);
	//PrimitiveGeometry& operator=(const PrimitiveGeometry& lhs);
	//PrimitiveGeometry& operator=(PrimitiveGeometry&& rhs);
	virtual ~PrimitiveGeometry();

	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	// temporary getters
	VertexBuffer<Vertex>& GetVertexBuffer();
	IndexBuffer& GetIndexBuffer();
	ConstantBuffer<ConstBuffer>& GetConstantBuffer();

	RenderComponent* GetRenderComponent() const;
	CollisionComponent* GetCollisionComponent() const;

	UINT stride = { 32 };
	UINT offset = { 0 };

protected:
	RenderComponent* pRenderComponent = nullptr;
	CollisionComponent* pCollisionComponent = nullptr;
private:
	std::vector<Vertex> vertices = 
	{
		{ DirectX::XMFLOAT4(0.2f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.2f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.2f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.2f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};
	std::vector<DWORD> indeces = { 0, 1, 2,  1, 0, 3 };

public:
	ConstantBuffer<ConstBuffer> constantBuffer;
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
};