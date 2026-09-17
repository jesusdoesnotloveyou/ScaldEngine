#pragma once

#include "ScaldActorComponent.h"
#include "Graphics/ScaldCoreTypes.h"
#include "Transform.h"

#include <vector>
#include <memory>

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

        }

        SceneComponent(std::shared_ptr<Actor> owner);
        virtual ~SceneComponent() override;
        virtual void Tick(float deltaTime) override;

    public:
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

        void SetScale(const XMVECTOR& scale);
        void SetScale(float x, float y, float z);
        void AdjustScale(const XMVECTOR& scale);
        void AdjustScale(float x, float y, float z);

        XMVECTOR GetForwardVector() const;
        XMVECTOR GetRightVector() const;
        XMVECTOR GetUpVector() const;

        void SetForwardVector(const XMVECTOR& relativeForwardVector);
        void SetRightVector(const XMVECTOR& relativeRightVector);
        void SetUpVector(const XMVECTOR& relativeUpVector);

        void AttachToParent(SceneComponent* parent);
        Transform& GetTransform();

    private:
        void UpdateAttachChildren();

    private:
        Transform m_transform;

        SceneComponent* m_attachParent = nullptr;
        std::vector<SceneComponent*> m_attachChildren;
    };
}  // namespace Scald