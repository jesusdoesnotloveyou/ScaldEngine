#include "stdafx.h"
#include "CollisionComponent.h"
#include "Objects/Geometry/Actor.h"
#include "Objects/Components/TransformComponent.h"

using namespace Scald;
using namespace DirectX;

CollisionComponent::CollisionComponent(SceneGeometry* Owner)
    : mOwnerObject(Owner),
      mCollisionTransform(Owner->GetTransform())
{
    // SetRadius(XMVectorGetX(Owner->GetScale()));
    OnCollisionOverlapSignature.AddRaw(this, &CollisionComponent::OnCollisionOverlap);
}

void CollisionComponent::Update(const ScaldTimer& st)
{
    if (!bIsEnabled) return;
    SetCenter(mOwnerObject->GetPosition());
    // SetRadius(XMVectorGetX(mOwnerObject->GetScale()) + GetRadius());
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

void CollisionComponent::Notify(CollisionComponent* otherCollisionComp)
{
    OnCollisionOverlapSignature.Broadcast(otherCollisionComp);
}

void CollisionComponent::OnCollisionOverlap(CollisionComponent* otherComponent)
{
    if (!otherComponent || !otherComponent->IsEnabled()) return;

    if (const auto player = GetOwner())
    {
        if (auto otherActor = static_cast<Actor*>(otherComponent->GetOwner()))
        {
            otherComponent->DisableCollision();
            otherActor->AttachToParent(player);
            // Increase Katamari size (should be moved to Katamari collision component as Katamari game specific logic)
            // player->AdjustScale(0.001f, 0.001f, 0.001f);
        }
    }
}