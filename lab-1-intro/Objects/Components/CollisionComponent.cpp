#include "CollisionComponent.h"

CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::Update(float DeltaTime)
{
	// TODO: lool at the UpdateOwnerTransform method
}

void CollisionComponent::Initialize(const STransform& ownerTransform)
{
	BoundingBox.Center = ownerTransform.Position;
	BoundingBox.Extents = ownerTransform.Scale;
}

DirectX::XMFLOAT3 CollisionComponent::GetCenter() const
{
	return BoundingBox.Center;
}

DirectX::XMFLOAT3 CollisionComponent::GetExtends() const
{
	return BoundingBox.Extents;
}

void CollisionComponent::UpdateOwnerTransform(const STransform& ownerTransform)
{
	BoundingBox.Center = ownerTransform.Position;
	BoundingBox.Extents = ownerTransform.Scale;
}

void CollisionComponent::SetExtends(const DirectX::XMFLOAT3& extends)
{
	BoundingBox.Extents = extends;
}

void CollisionComponent::SetCenter(const DirectX::XMFLOAT3& center)
{
	BoundingBox.Center = center;
}