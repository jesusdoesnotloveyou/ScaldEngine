#pragma once

#include "../../../Graphics/VertexBuffer.h"
#include "../../../Graphics/IndexBuffer.h"
#include "../../../Graphics/ConstantBuffer.h"
#include "../../../Graphics/ScaldCoreTypes.h"

#include "../../../ScaldCore/Engine/ScaldTimer.h"

#include "../../../Objects/Components/RenderComponent.h"
#include "../../../Objects/Components/CollisionComponent.h"
#include "../../../Objects/Components/InputComponent.h"
#include "../../../Objects/Components/MovementComponent.h"

#include <cmath>
#include <vector>

class RenderComponent;
class CollisionComponent;
class InputComponent;
class MovementComponent;

class PrimitiveGeometry
{
public:
	PrimitiveGeometry();
	PrimitiveGeometry(const STransform& transform);
	PrimitiveGeometry(const std::tuple<std::vector<Vertex>, std::vector<DWORD>>& viPair);
	// would be changed to normal constructor
	virtual ~PrimitiveGeometry();

	virtual void Update(const ScaldTimer& st) = 0;
	virtual void Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext) = 0;

public:
	VertexBuffer<Vertex>& GetVertexBuffer();
	IndexBuffer& GetIndexBuffer();
	ConstantBuffer<ConstBuffer>& GetConstantBuffer();

	RenderComponent* GetRenderComponent() const;
	CollisionComponent* GetCollisionComponent() const;
	MovementComponent* GetMovementComponent() const;

	UINT stride = { 32 }; // sizeof Vertex structure
	UINT offset = { 0 };

	STransform ObjectTransform;

protected:
	RenderComponent* pRenderComponent = nullptr;
	CollisionComponent* pCollisionComponent = nullptr;
	InputComponent* pInputComponent = nullptr;
	MovementComponent* pMovementComponent = nullptr;

	std::vector<Vertex> vertices;
	std::vector<DWORD> indeces;

private:
	ConstantBuffer<ConstBuffer> constantBuffer;
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
};