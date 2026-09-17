#pragma once

#include "GameFramework/Components/ScaldActorComponent.h"
#include "Graphics//DXHelper.h"
#include "Delegates/Delegates.h"
#include "GameFramework/Components/Transform.h"

#include <memory>

namespace Scald
{
    using namespace DirectX;

    class Actor;

    class CollisionComponent : public ScaldActorComponent
    {
        using Super = ScaldActorComponent;
    public:
        CollisionComponent();
        CollisionComponent(std::shared_ptr<Actor> owner);
        virtual ~CollisionComponent() noexcept override;

        virtual void Tick(float deltaTime) override;

        void SetCenter(const XMFLOAT3& center);
        void SetCenter(const XMVECTOR& center);
        FORCEINLINE XMFLOAT3 GetCenter() const { return m_boundingVolume.Center; }

        void SetRadius(const float radius);
        FORCEINLINE float GetRadius() const { return m_boundingVolume.Radius; }

        FORCEINLINE BoundingSphere& GetBoundingVolume() { return m_boundingVolume; }

        bool Intersects(CollisionComponent* otherComponent);
        
        FORCEINLINE bool IsEnabled() const { return bIsEnabled; }
        FORCEINLINE void DisableCollision() { bIsEnabled = false; }

        void Notify(CollisionComponent* otherCollisionComp);

    protected:
        void OnCollisionOverlap(CollisionComponent* otherComponent);

    private:
        MulticastDelegate<CollisionComponent*> OnCollisionOverlapSignature;

        BoundingSphere m_boundingVolume;
        Transform m_collisionTransform;
    
        bool bIsEnabled = true;
    };
}