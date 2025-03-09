#pragma once

#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/IndexBuffer.h"
#include "../../Graphics/ConstantBuffer.h"
#include "../../Graphics/ScaldCoreTypes.h"

#include "../../Objects/Components/RenderComponent.h"
#include "../../Objects/Components/CollisionComponent.h"
#include "../../Objects/Components/InputComponent.h"

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
	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) = 0;

public:
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
	InputComponent* pInputComponent = nullptr;

	std::vector<Vertex> vertices;
	std::vector<DWORD> indeces;

private:
	ConstantBuffer<ConstBuffer> constantBuffer;
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
};