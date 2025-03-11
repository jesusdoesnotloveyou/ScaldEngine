#include "CollisionComponent.h"

CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::Update(float DeltaTime)
{

}

void CollisionComponent::Initialize(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extends)
{
	BoundingBox.Center = center;
	BoundingBox.Extents = extends;
}

DirectX::XMFLOAT3 CollisionComponent::GetCenter() const
{
	return BoundingBox.Center;
}

DirectX::XMFLOAT3 CollisionComponent::GetExtends() const
{
	return BoundingBox.Extents;
}
