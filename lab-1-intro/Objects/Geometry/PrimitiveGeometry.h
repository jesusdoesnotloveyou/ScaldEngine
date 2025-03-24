#pragma once

#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/IndexBuffer.h"
#include "../../Graphics/ConstantBuffer.h"
#include "../../Graphics/ScaldCoreTypes.h"

#include "../../Objects/Components/RenderComponent.h"
#include "../../Objects/Components/CollisionComponent.h"
#include "../../Objects/Components/InputComponent.h"
#include "../../Objects/Components/MovementComponent.h"

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
	// would be changed to normal constructor
	PrimitiveGeometry(const std::vector<Vertex>& v, const std::vector<DWORD>& i);
	virtual ~PrimitiveGeometry();

	virtual void Update(float DeltaTime) = 0;
	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) = 0;

public:
	VertexBuffer<Vertex>& GetVertexBuffer();
	IndexBuffer& GetIndexBuffer();
	ConstantBuffer<ConstBuffer>& GetConstantBuffer();

	RenderComponent* GetRenderComponent() const;
	CollisionComponent* GetCollisionComponent() const;
	MovementComponent* GetMovementComponent() const;

	void SetIsMovable(bool newStatus) { bIsMovable = newStatus; }
	bool GetIsMovable() const { return bIsMovable; }

	UINT stride = { 32 }; // sizeof Vertex structure
	UINT offset = { 0 };

	STransform ObjectTransform;

	// Pong specific
	void Reset(const XMFLOAT3& newSpeed, const XMFLOAT3& newTranslation);

	// for bonus (should be enum or smth)
	bool bIsSpeedIncrease = false;
	bool bIsDirectionChanger = false;

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

	bool bIsMovable = false;

	int leftPlayerScore = 0;
	int rightPlayerScore = 0;
};