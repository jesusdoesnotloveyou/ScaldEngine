#include "stdafx.h"
#include "CollisionComponent.h"
#include "GameFramework/Objects/Actor.h"
#include "GameFramework/Components/Transform.h"

#include <memory>

using namespace Scald;
using namespace DirectX;

CollisionComponent::CollisionComponent()
    : ScaldActorComponent()
{

}

CollisionComponent::CollisionComponent(std::shared_ptr<Actor> owner)
    : ScaldActorComponent(owner)
    //, m_collisionTransform(owner->GetTransform())
{
    // SetRadius(XMVectorGetX(owner->GetScale()));
    OnCollisionOverlapSignature.AddRaw(this, &CollisionComponent::OnCollisionOverlap);
}

CollisionComponent::~CollisionComponent()
{

}

void CollisionComponent::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    
    if (!bIsEnabled) return;
    //SetCenter(m_owner->GetPosition());
    // SetRadius(XMVectorGetX(mOwnerObject->GetScale()) + GetRadius());
}

void CollisionComponent::SetRadius(float radius)
{
    m_boundingVolume.Radius = radius;
}

void CollisionComponent::SetCenter(const XMFLOAT3& center)
{
    m_boundingVolume.Center = center;
}

void CollisionComponent::SetCenter(const XMVECTOR& center)
{
    XMStoreFloat3(&m_boundingVolume.Center, center);
}

bool CollisionComponent::Intersects(CollisionComponent* otherComponent)
{
    if (otherComponent)
    {
        return m_boundingVolume.Intersects(otherComponent->GetBoundingVolume());
    }
    return false;
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
            //otherActor->AttachToParent(player);
            // Increase Katamari size (should be moved to Katamari collision component as Katamari game specific logic)
            // player->AdjustScale(0.001f, 0.001f, 0.001f);
        }
    }
}