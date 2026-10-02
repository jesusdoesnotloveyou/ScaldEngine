#pragma once

#include "ScaldActorComponent.h"
#include "Graphics/ScaldCoreTypes.h"
#include "Transform.h"
#include "Delegates/Delegates.h"

#include <vector>
#include <memory>
#include <cstdint>

namespace Scald
{
    class Actor;
    
    // SceneComponent has a transform evaluated every frame and supports attachment, but has no rendering or collision capabilities.
    // Typical components that inherit from SceneComponent are: Camera, Audio, etc.
    class SceneComponent : public ScaldActorComponent
    {
        using Super = ScaldActorComponent;
    public:
        SceneComponent() 
            : ScaldActorComponent()
        {
            m_componentToWorld = Transform::Identity;
        }

        SceneComponent(std::shared_ptr<Actor> owner);
        virtual ~SceneComponent() override;
        virtual void Tick(float deltaTime) override;
        virtual void OnRegister() override;
        virtual void OnUnregister() override;
    public:
        /** Get the current component-to-world transform for this component */
        const Transform& GetComponentTransform() const;

        XMVECTOR GetPosition() const;
        XMFLOAT3 GetPositionFloat() const;
        XMVECTOR GetRotation() const;
        XMVECTOR GetOrientation() const;
        XMVECTOR GetScale() const;

        virtual void SetPosition(const XMVECTOR& pos);
        virtual void SetPosition(float x, float y, float z);
        virtual void AdjustPosition(const XMVECTOR& pos);
        virtual void AdjustPosition(float x, float y, float z);

        virtual void SetOrientation(const XMVECTOR& newRotation);
        virtual void SetRotation(const XMVECTOR& rot);
        virtual void SetRotation(float x, float y, float z);
        virtual void AdjustRotation(const XMVECTOR& rot);
        virtual void AdjustRotation(float x, float y, float z);

        virtual void SetScale(const XMVECTOR& scale);
        virtual void SetScale(float x, float y, float z);
        virtual void AdjustScale(const XMVECTOR& scale);
        virtual void AdjustScale(float x, float y, float z);

        /** Convenience function to get the relative rotation from the passed in world rotation */
        Quaternion GetRelativeRotationFromWorld(const Quaternion& WorldRotation);

        /** Set the location of the component relative to its parent */
        void SetRelativeLocation(Vector3 NewLocation);

        /** Set the rotation of the component relative to its parent */
        void SetRelativeRotation(Vector3 NewRotator);
        void SetRelativeRotation(const Quaternion& NewRotation);

        /** Set the transform of the component relative to its parent */
        void SetRelativeTransform(const Transform& NewTransform);

        /** Returns the transform of the component relative to its parent */
        Transform GetRelativeTransform() const;

        /** Reset the transform of the component relative to its parent. Sets relative location to zero, relative rotation to no rotation, and Scale to 1. */
        void ResetRelativeTransform();

        /** Set the non-uniform scale of the component relative to its parent */
        void SetRelativeScale3D(Vector3 NewScale3D);

        /** Adds a delta to the translation of the component relative to its parent */
        void AddRelativeLocation(Vector3 DeltaLocation);

        /** Adds a delta the rotation of the component relative to its parent */
        void AddRelativeRotation(Vector3 DeltaRotator);
        void AddRelativeRotation(const Quaternion& DeltaRotation);

        /** Adds a delta to the location of the component in its local reference frame */
        void AddLocalOffset(Vector3 DeltaLocation);

        /** Adds a delta to the rotation of the component in its local reference frame */
        void AddLocalRotation(Vector3 DeltaRotator);
        void AddLocalRotation(const Quaternion& DeltaRotation);

        /** Adds a delta to the transform of the component in its local reference frame. Scale is unchanged. */
        void AddLocalTransform(const Transform& DeltaTransform);

        /** Put this component at the specified location in world space. Updates relative location to achieve the final world location. */
        void SetWorldLocation(Vector3 NewLocation);

        /* Put this component at the specified rotation in world space. Updates relative rotation to achieve the final world rotation. */
        void SetWorldRotation(Vector3 NewRotator);
        void SetWorldRotation(const Quaternion& NewRotation);

        /** Set the relative scale of the component to put it at the supplied scale in world space. */
        void SetWorldScale3D(Vector3 NewScale);

        /** Set the transform of the component in world space. */
        void SetWorldTransform(const Transform& NewTransform);

        /** Adds a delta to the location of the component in world space. */
        void AddWorldOffset(Vector3 DeltaLocation);

        /** Adds a delta to the rotation of the component in world space. */
        void AddWorldRotation(Vector3 DeltaRotation);
        void AddWorldRotation(const Quaternion& DeltaRotation);

        /** Adds a delta to the transform of the component in world space. Ignores scale and sets it to (1,1,1). */
        void AddWorldTransform(const Transform& DeltaTransform);

        /** Return location of the component, in world space */
        XMVECTOR GetComponentLocation() const;

        /** Returns rotation of the component, in world space. */
        XMVECTOR GetComponentRotation() const;

        /** Returns scale of the component, in world space. */
        XMVECTOR GetComponentScale() const;

        /** Get the forward (Z) unit direction vector from this component, in world space.  */
        XMVECTOR GetForwardVector() const;
        /** Get the right (X) unit direction vector from this component, in world space.  */
        XMVECTOR GetRightVector() const;
        /** Get the up (Y) unit direction vector from this component, in world space.  */
        XMVECTOR GetUpVector() const;

        virtual void SetupAttachment(SceneComponent* inParent, const char* inSocketName = "");
        virtual void DetachFromComponent();
        FORCEINLINE bool IsAttached() const { return m_attachParent != nullptr; }

    private:
        void AttachToParent(SceneComponent* parent);
        void UpdateAttachChildren();

        void UpdateComponentToWorld();

    private:
        /** List of child SceneComponents that are attached to us. */
        std::vector<SceneComponent*> m_attachChildren;

        /** Current transform of the component, relative to the world */
        Transform m_componentToWorld;

        /** What we are currently attached to. If valid, RelativeLocation etc. are used relative to this object */
        SceneComponent* m_attachParent;

    private:
        /** Location of the component relative to its parent */
        XMFLOAT3 RelativeLocation;

        /** Rotation of the component relative to its parent */
        XMFLOAT3 RelativeRotation;

        /**
         *	Non-uniform scaling of the component relative to its parent.
         *	Note that scaling is always applied in local space (no shearing etc)
         */
        XMFLOAT3 RelativeScale3D;

    protected:
        /** True if we have ever updated ComponentToWorld based on RelativeLocation/Rotation/Scale. Used at startup to make sure it is initialized. */
        uint8_t bComponentToWorldUpdated : 1;

        bool m_bIsDirty = false;

    public:
        /** Delegate called when this component is moved */
        MulticastDelegate<SceneComponent*> TransformUpdated;
    };
}  // namespace Scald