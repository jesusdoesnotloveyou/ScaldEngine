#include "stdafx.h"
#include "Transform.h"

using namespace Scald;

const Transform Transform::Identity = {};

Transform::Transform()
{
    mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    mRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    mScaleVector = XMVectorSet(mScale.x, mScale.y, mScale.z, 0.0f);
    mEulerRotation = XMVectorSet(mRot.x, mRot.y, mRot.z, 0.0f);
    mPosVector = XMVectorSet(mPos.x, mPos.y, mPos.z, 0.0f);
    mQuaternionRotation = XMQuaternionIdentity();
}

void Transform::SetWorldMatrix(const XMMATRIX& worldMat)
{
    mWorldMatrix = worldMat;
}

void Transform::Reset()
{
    mWorldMatrix = XMMatrixIdentity();
}

XMVECTOR Transform::GetPositionVector() const
{
    return mPosVector;
}

XMFLOAT3 Transform::GetPositionFloat3() const
{
    return mPos;
}

XMVECTOR Transform::GetRotationVector() const
{
    return mEulerRotation;
}

XMVECTOR Transform::GetOrientation() const
{
    return mQuaternionRotation;
}

XMFLOAT3 Transform::GetRotationFloat3() const
{
    return mRot;
}

XMVECTOR Transform::GetScaleVector() const
{
    return mScaleVector;
}

XMFLOAT3 Transform::GetScaleFloat3() const
{
    return mScale;
}

void Transform::SetScale(const XMVECTOR& scaleVector)
{
    mScaleVector = scaleVector;
    XMStoreFloat3(&mScale, mScaleVector);
    UpdateWorldMatrix();
}

void Transform::SetScale(const XMFLOAT3& scale)
{
    mScale = scale;
    mScaleVector = XMLoadFloat3(&mScale);
    UpdateWorldMatrix();
}

void Transform::SetScale(float x, float y, float z)
{
    mScale = XMFLOAT3(x, y, z);
    mScaleVector = XMLoadFloat3(&mScale);
    UpdateWorldMatrix();
}

void Transform::AdjustScale(const XMVECTOR& scaleVector)
{
    mScaleVector += scaleVector;
    XMStoreFloat3(&mScale, mScaleVector);
    UpdateWorldMatrix();
}

void Transform::AdjustScale(const XMFLOAT3& scale)
{
    mScale.x += scale.x;
    mScale.y += scale.y;
    mScale.z += scale.z;
    mScaleVector = XMLoadFloat3(&mScale);
    UpdateWorldMatrix();
}

void Transform::AdjustScale(float x, float y, float z)
{
    mScale.x += x;
    mScale.y += y;
    mScale.z += z;
    mScaleVector = XMLoadFloat3(&mScale);
    UpdateWorldMatrix();
}

void Transform::SetPosition(const XMVECTOR& posVector)
{
    mPosVector = posVector;
    XMStoreFloat3(&mPos, mPosVector);
    UpdateWorldMatrix();
}

void Transform::SetPosition(const XMFLOAT3& pos)
{
    mPos = pos;
    mPosVector = XMLoadFloat3(&mPos);
    UpdateWorldMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
    mPos = XMFLOAT3(x, y, z);
    mPosVector = XMLoadFloat3(&mPos);
    UpdateWorldMatrix();
}

void Transform::AdjustPosition(const XMVECTOR& posVector)
{
    mPosVector += posVector;
    XMStoreFloat3(&mPos, mPosVector);
    UpdateWorldMatrix();
}

void Transform::AdjustPosition(const XMFLOAT3& pos)
{
    mPos.x += pos.x;
    mPos.y += pos.y;
    mPos.z += pos.z;
    mPosVector = XMLoadFloat3(&mPos);
    UpdateWorldMatrix();
}

void Transform::AdjustPosition(float x, float y, float z)
{
    mPos.x += x;
    mPos.y += y;
    mPos.z += z;
    mPosVector = XMLoadFloat3(&mPos);
    UpdateWorldMatrix();
}

void Transform::SetOrientation(const XMVECTOR& newRotation)
{
    mQuaternionRotation = XMQuaternionMultiply(mQuaternionRotation, newRotation);
    UpdateWorldMatrix();
}

void Transform::SetRotation(const XMVECTOR& rotVector)
{
    mEulerRotation = rotVector;
    XMStoreFloat3(&mRot, mEulerRotation);
    UpdateWorldMatrix();
}

void Transform::SetRotation(const XMFLOAT3& rot)
{
    mRot = rot;
    mEulerRotation = XMLoadFloat3(&mRot);
    UpdateWorldMatrix();
}

void Transform::SetRotation(float x, float y, float z)
{
    mRot = XMFLOAT3(x, y, z);
    mEulerRotation = XMLoadFloat3(&mRot);
    UpdateWorldMatrix();
}

void Transform::AdjustRotation(const XMVECTOR& rotVector)
{
    mEulerRotation += rotVector;
    XMStoreFloat3(&mRot, mEulerRotation);
    UpdateWorldMatrix();
}

void Transform::AdjustRotation(const XMFLOAT3& rot)
{
    mRot.x += rot.x;
    mRot.y += rot.y;
    mRot.z += rot.z;
    mEulerRotation = XMLoadFloat3(&mRot);
    UpdateWorldMatrix();
}

void Transform::AdjustRotation(float x, float y, float z)
{
    mRot.x += x;
    mRot.y += y;
    mRot.z += z;
    mEulerRotation = XMLoadFloat3(&mRot);
    UpdateWorldMatrix();
}

XMVECTOR Transform::GetForwardVector() const
{
    return mForwardVector;
}

XMVECTOR Transform::GetRightVector() const
{
    return mRightVector;
}

XMVECTOR Transform::GetUpVector() const
{
    return mUpVector;
}

void Transform::SetForwardVector(const XMVECTOR& ForwardVector)
{
    mForwardVector = ForwardVector;
}

void Transform::SetRightVector(const XMVECTOR& RightVector)
{
    mRightVector = RightVector;
}

void Transform::SetUpVector(const XMVECTOR& UpVector)
{
    mUpVector = UpVector;
}

void Transform::SetParentTransform(const Transform& parentTransform)
{

}

void Transform::UpdateWorldMatrix()
{
    // SRT - default order of matrix multiplication
    // (S)TR - orbit effect for Solar system could be used
    mWorldMatrix = XMMatrixScalingFromVector(mScaleVector) * XMMatrixRotationQuaternion(mQuaternionRotation) * XMMatrixTranslationFromVector(mPosVector);

    if (m_parentTransform)
    {
        mWorldMatrix *= XMMatrixRotationQuaternion(m_parentTransform->GetOrientation()) * 
                        XMMatrixTranslationFromVector(m_parentTransform->GetPositionVector());
    }
}