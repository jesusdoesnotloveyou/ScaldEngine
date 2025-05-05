#include "CollisionComponent.h"
#include "../../Geometry/Actor.h"
#include "../TransformComponent.h"

CollisionComponent::CollisionComponent(SceneGeometry* Owner)
	: 
	mOwnerObject(Owner),
	mCollisionTransform(Owner->GetTransform())
{
	//SetRadius(XMVectorGetX(Owner->GetScale()));
	OnCollisionOverlapSignature.AddRaw(this, &CollisionComponent::OnCollisionOverlap);
}

void CollisionComponent::Update(const ScaldTimer& st)
{
	if (!bIsEnabled) return;
	SetCenter(mOwnerObject->GetPosition());
	//SetRadius(XMVectorGetX(mOwnerObject->GetScale()) + GetRadius());
}

void CollisionComponent::SetRadius(float radius)
{
	mBoundingVolume.Radius = radius;
}

void CollisionComponent::SetCenter(const XMFLOAT3& center)
{
	mBoundingVolume.Center = center;
}

void CollisionComponent::SetCenter(const XMVECTOR& center)
{
	XMStoreFloat3(&mBoundingVolume.Center, center);
}

bool CollisionComponent::Intersects(CollisionComponent* otherComponent)
{
	if (otherComponent)
	{
		return mBoundingVolume.Intersects(otherComponent->GetBoundingVolume());
	}
	return false;
}

SceneGeometry* CollisionComponent::GetOwner() const
{
	return mOwnerObject ? mOwnerObject : nullptr;
}

void CollisionComponent::OnCollisionOverlap(CollisionComponent* otherComponent)
{
	if (!otherComponent || !otherComponent->IsEnabled()) return;

	const auto player = GetOwner();
	// doesn't work properly
	//if (player->GetCollisionComponent()->GetRadius() < otherComponent->GetRadius()) return;

	if (auto actor = static_cast<Actor*>(otherComponent->GetOwner()))
	{
		otherComponent->DisableCollision();
		actor->AttachToParent(player);
		
		//player->AdjustScale(0.001f, 0.001f, 0.001f);
	}
}