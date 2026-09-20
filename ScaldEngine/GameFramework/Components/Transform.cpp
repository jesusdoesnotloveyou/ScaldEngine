#include "stdafx.h"
#include "Transform.h"

using namespace Scald;

const Transform Transform::Identity = Transform(
    XMFLOAT3(1.0f, 1.0f, 1.0f),
    XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
    XMFLOAT3(1.0f, 1.0f, 1.0f)
);

Transform::Transform()
{
    mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    mEulerRotator = XMFLOAT3(0.0f, 0.0f, 0.0f);
    mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    mQuaternionRotation = XMQuaternionIdentity();
}

XMVECTOR Transform::GetPositionVector() const
{
    return XMLoadFloat3(&mPos);
}

XMFLOAT3 Transform::GetPositionFloat3() const
{
    return mPos;
}

XMVECTOR Transform::GetRotationVector() const
{
    return XMLoadFloat3(&mEulerRotator);
}

XMVECTOR Transform::GetOrientation() const
{
    return mQuaternionRotation;
}

XMFLOAT3 Transform::GetRotationFloat3() const
{
    return mEulerRotator;
}

XMVECTOR Transform::GetScaleVector() const
{
    return XMLoadFloat3(&mScale);
}

XMFLOAT3 Transform::GetScaleFloat3() const
{
    return mScale;
}

void Transform::SetScale(const XMVECTOR& scaleVector)
{
    XMStoreFloat3(&mScale, scaleVector);
}

void Transform::SetScale(const XMFLOAT3& scale)
{
    mScale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
    mScale = XMFLOAT3(x, y, z);
}

void Transform::AdjustScale(const XMVECTOR& scaleVector)
{
    XMFLOAT3 deltaScale;
    XMStoreFloat3(&deltaScale, scaleVector);
    AdjustScale(deltaScale);
}

void Transform::AdjustScale(const XMFLOAT3& scale)
{
    mScale.x += scale.x;
    mScale.y += scale.y;
    mScale.z += scale.z;
}

void Transform::AdjustScale(float x, float y, float z)
{
    mScale.x += x;
    mScale.y += y;
    mScale.z += z;
}

void Transform::SetPosition(const XMVECTOR& posVector)
{
    XMStoreFloat3(&mPos, posVector);
}

void Transform::SetPosition(const XMFLOAT3& pos)
{
    mPos = pos;
}

void Transform::SetPosition(float x, float y, float z)
{
    mPos = XMFLOAT3(x, y, z);
}

void Transform::AdjustPosition(const XMVECTOR& posVector)
{
    XMFLOAT3 deltaPos;
    XMStoreFloat3(&deltaPos, posVector);
    AdjustPosition(deltaPos);
}

void Transform::AdjustPosition(const XMFLOAT3& pos)
{
    mPos.x += pos.x;
    mPos.y += pos.y;
    mPos.z += pos.z;
}

void Transform::AdjustPosition(float x, float y, float z)
{
    mPos.x += x;
    mPos.y += y;
    mPos.z += z;
}

void Transform::SetOrientation(const XMVECTOR& newRotation)
{
    mQuaternionRotation = XMQuaternionMultiply(mQuaternionRotation, newRotation);
}

void Transform::SetRotation(const XMVECTOR& rotVector)
{
    XMStoreFloat3(&mEulerRotator, rotVector);
}

void Transform::SetRotation(const XMFLOAT3& rot)
{
    mEulerRotator = rot;
}

void Transform::SetRotation(float x, float y, float z)
{
    mEulerRotator = XMFLOAT3(x, y, z);
}

void Transform::AdjustRotation(const XMVECTOR& rotVector)
{
    XMFLOAT3 deltaRotation;
    XMStoreFloat3(&deltaRotation, rotVector);
    AdjustRotation(deltaRotation);
}

void Transform::AdjustRotation(const XMFLOAT3& rot)
{
    mEulerRotator.x += rot.x;
    mEulerRotator.y += rot.y;
    mEulerRotator.z += rot.z;
}

void Transform::AdjustRotation(float x, float y, float z)
{
    mEulerRotator.x += x;
    mEulerRotator.y += y;
    mEulerRotator.z += z;
}

XMVECTOR Transform::GetForwardVector() const
{
    return XMVECTOR{};
}

XMVECTOR Transform::GetRightVector() const
{
    return XMVECTOR{};
}

XMVECTOR Transform::GetUpVector() const
{
    return XMVECTOR{};
}