#pragma once

#include "Graphics//DXHelper.h"
#include "Objects/Components/ScaldComponent.h"
#include "Delegates/Delegates.h"

namespace Scald
{
    class SceneGeometry;
    class TransformComponent;

    class CollisionComponent : public ScaldComponent
    {
    public:
        CollisionComponent(SceneGeometry* Owner);
        virtual ~CollisionComponent() noexcept override {}
        virtual void Update(const ScaldTimer& st) override;

        void SetCenter(const XMFLOAT3& center);
        void SetCenter(const XMVECTOR& center);
        FORCEINLINE XMFLOAT3 GetCenter() const { return mBoundingVolume.Center; }

        void SetRadius(const float radius);
        FORCEINLINE float GetRadius() const { return mBoundingVolume.Radius; }

        FORCEINLINE BoundingSphere& GetBoundingVolume() { return mBoundingVolume; }

        bool Intersects(CollisionComponent* otherComponent);
        SceneGeometry* GetOwner() const;

        FORCEINLINE bool IsEnabled() const { return bIsEnabled; }
        FORCEINLINE void DisableCollision() { bIsEnabled = false; }

        void Notify(CollisionComponent* otherCollisionComp);

    protected:
        void OnCollisionOverlap(CollisionComponent* otherComponent);

    private:
        MulticastDelegate<CollisionComponent*> OnCollisionOverlapSignature;

        BoundingSphere mBoundingVolume;
        SceneGeometry* mOwnerObject = nullptr;
        TransformComponent* mCollisionTransform = nullptr;
    
        bool bIsEnabled = true;
    };
}