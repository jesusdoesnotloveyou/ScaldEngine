#pragma once

#include "ScaldComponent.h"
#include "Graphics/ScaldCoreTypes.h"

#include <vector>

namespace Scald
{
    class TransformComponent;

    class SceneComponent : public ScaldComponent
    {
    public:
        SceneComponent();
        virtual ~SceneComponent() override;
        virtual void Update(const ScaldTimer& st) override;

    public:
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

        void AttachToParent(SceneComponent* Parent);
        FORCEINLINE SceneComponent* GetParent() { return mParent; }

        FORCEINLINE SceneComponent* GetRootObject()
        {
            return mParent ? mParent->GetRootObject() : this;
        }

        TransformComponent* GetTransform() const;

    private:
        SceneComponent* mParent = nullptr;
        TransformComponent* mTransformComponent = nullptr;
        std::vector<SceneComponent*> mChildren{};
    };
}  // namespace Scald