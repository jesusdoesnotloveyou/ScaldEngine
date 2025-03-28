#include "CollisionComponent.h"

DirectX::XMFLOAT3 CollisionComponent::GetCenter() const
{
	return BoundingBox.Center;
}

DirectX::XMFLOAT3 CollisionComponent::GetExtends() const
{
	return BoundingBox.Extents;
}

void CollisionComponent::SetExtends(const DirectX::XMFLOAT3& extends)
{
	BoundingBox.Extents = extends;
}

void CollisionComponent::SetCenter(const DirectX::XMFLOAT3& center)
{
	BoundingBox.Center = center;
}