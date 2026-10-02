#pragma once

#include "Graphics/ScaldCoreTypes.h"
#include "Math/ScaldMath.h"

namespace Scald
{
    struct Transform
    {
    public:
        Transform();
        Transform(const Transform& t) = default;
        Transform(Transform&& t) noexcept = default;
        Transform& operator=(const Transform& t) = default;
        Transform& operator=(Transform&& t) noexcept = default;

        Transform(const XMFLOAT3& scale, const XMVECTOR& rotation, const XMFLOAT3& location)
        {
            mScale = scale;
            mQuaternionRotation = rotation;
            mPos = location;
        }
        static const Transform Identity;

    public:
        XMVECTOR GetPositionVector() const;
        XMFLOAT3 GetPositionFloat3() const;
        XMVECTOR GetOrientation() const;
        XMVECTOR GetRotationVector() const;
        XMFLOAT3 GetRotationFloat3() const;
        XMVECTOR GetScaleVector() const;
        XMFLOAT3 GetScaleFloat3() const;

        void SetScale(const XMVECTOR& scaleVector);
        void SetScale(const XMFLOAT3& scale);
        void SetScale(float x, float y, float z);
        void AdjustScale(const XMVECTOR& scaleVector);
        void AdjustScale(const XMFLOAT3& scale);
        void AdjustScale(float x, float y, float z);

        void SetOrientation(const XMVECTOR& orient);

        void SetRotation(const XMVECTOR& rotVector);
        void SetRotation(const XMFLOAT3& rot);
        void SetRotation(float x, float y, float z);
        void AdjustRotation(const XMVECTOR& rotVector);
        void AdjustRotation(const XMFLOAT3& rot);
        void AdjustRotation(float x, float y, float z);

        void SetPosition(const XMVECTOR& posVector);
        void SetPosition(const XMFLOAT3& pos);
        void SetPosition(float x, float y, float z);
        void AdjustPosition(const XMVECTOR& posVector);
        void AdjustPosition(const XMFLOAT3& pos);
        void AdjustPosition(float x, float y, float z);

        // Placeholders
        XMVECTOR GetForwardVector() const;
        XMVECTOR GetRightVector() const;
        XMVECTOR GetUpVector() const;

        //void SetForwardVector(const XMVECTOR& ForwardVector);
        //void SetRightVector(const XMVECTOR& RightVector);
        //void SetUpVector(const XMVECTOR& UpVector);
    private:
        // Need to init
        XMFLOAT3 mScale         = {1.0f, 1.0f, 1.0f};
        XMFLOAT3 mEulerRotator  = {0.0f, 0.0f, 0.0f};
        XMFLOAT3 mPos           = {0.0f, 0.0f, 0.0f};
        // TODO: we can remove euler rotator and only use quaternion thereby saving 12 bytes
        XMVECTOR mQuaternionRotation;
    };
}