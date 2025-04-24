#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	mScaleMatrix = XMMatrixIdentity();
	mRotationMatrix = XMMatrixIdentity();
	mOrientationQuat = XMQuaternionIdentity();
	mTranslationMatrix = XMMatrixIdentity();
	mWorldMatrix = XMMatrixIdentity();
	mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mScaleVector = XMVectorSet(mScale.x, mScale.y, mScale.z, 0.0f);
	mRotVector = XMVectorSet(mRot.x, mRot.y, mRot.z, 0.0f);
	mPosVector = XMVectorSet(mPos.x, mPos.y, mPos.z, 0.0f);
}

void TransformComponent::Update(const ScaldTimer& st)
{
	// Bad for perfomance
	//UpdateWorldMatrix();
}

void TransformComponent::SetWorldMatrix(const XMMATRIX& worldMat)
{
	mWorldMatrix = worldMat;
}

void TransformComponent::Reset()
{
	mWorldMatrix = XMMatrixIdentity();
}

XMVECTOR TransformComponent::GetPositionVector() const
{
	return mPosVector;
}

XMFLOAT3 TransformComponent::GetPositionFloat3() const
{
	return mPos;
}

XMVECTOR TransformComponent::GetRotationVector() const
{
	return mRotVector;
}

XMVECTOR TransformComponent::GetOrientation() const 
{
	return mOrientationQuat;
}

XMFLOAT3 TransformComponent::GetRotationFloat3() const
{
	return mRot;
}

XMVECTOR TransformComponent::GetScaleVector() const
{
	return mScaleVector;
}

XMFLOAT3 TransformComponent::GetScaleFloat3() const
{
	return mScale;
}

void TransformComponent::SetScale(const XMVECTOR& scaleVector)
{
	mScaleVector = scaleVector;
	XMStoreFloat3(&mScale, mScaleVector);
	UpdateWorldMatrix();
}

void TransformComponent::SetScale(const XMFLOAT3& scale)
{
	mScale = scale;
	mScaleVector = XMLoadFloat3(&mScale);
	UpdateWorldMatrix();
}

void TransformComponent::SetScale(float x, float y, float z)
{
	mScale = XMFLOAT3(x, y, z);
	mScaleVector = XMLoadFloat3(&mScale);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustScale(const XMVECTOR& scaleVector)
{
	mScaleVector += scaleVector;
	XMStoreFloat3(&mScale, mScaleVector);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustScale(const XMFLOAT3& scale)
{
	mScale.x += scale.x;
	mScale.y += scale.y;
	mScale.z += scale.z;
	mScaleVector = XMLoadFloat3(&mScale);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustScale(float x, float y, float z)
{
	mScale.x += x;
	mScale.y += y;
	mScale.z += z;
	mScaleVector = XMLoadFloat3(&mScale);
	UpdateWorldMatrix();
}

void TransformComponent::SetPosition(const XMVECTOR& posVector)
{
	mPosVector = posVector;
	XMStoreFloat3(&mPos, mPosVector);
	UpdateWorldMatrix();
}

void TransformComponent::SetPosition(const XMFLOAT3& pos)
{
	mPos = pos;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	mPos = XMFLOAT3(x, y, z);
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustPosition(const XMVECTOR& posVector)
{
	mPosVector += posVector;
	XMStoreFloat3(&mPos, mPosVector);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustPosition(const XMFLOAT3& pos)
{
	mPos.x += pos.x;
	mPos.y += pos.y;
	mPos.z += pos.z;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustPosition(float x, float y, float z)
{
	mPos.x += x;
	mPos.y += y;
	mPos.z += z;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void TransformComponent::SetOrientation(const XMVECTOR& newRotation)
{
	mOrientationQuat = XMQuaternionMultiply(mOrientationQuat, newRotation);
	UpdateWorldMatrix();
}

void TransformComponent::SetRotation(const XMVECTOR& rotVector)
{
	mRotVector = rotVector;
	XMStoreFloat3(&mRot, mRotVector);
	UpdateWorldMatrix();
}

void TransformComponent::SetRotation(const XMFLOAT3& rot)
{
	mRot = rot;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

void TransformComponent::SetRotation(float x, float y, float z)
{
	mRot = XMFLOAT3(x, y, z);
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustRotation(const XMVECTOR& rotVector)
{
	mRotVector += rotVector;
	XMStoreFloat3(&mRot, mRotVector);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustRotation(const XMFLOAT3& rot)
{
	mRot.x += rot.x;
	mRot.y += rot.y;
	mRot.z += rot.z;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

void TransformComponent::AdjustRotation(float x, float y, float z)
{
	mRot.x += x;
	mRot.y += y;
	mRot.z += z;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

XMVECTOR TransformComponent::GetForwardVector()const
{
	return mForwardVector;
}

XMVECTOR TransformComponent::GetRightVector()const
{
	return mRightVector;
}

XMVECTOR TransformComponent::GetUpVector() const
{
	return mUpVector;
}

void TransformComponent::SetForwardVector(const XMVECTOR& ForwardVector)
{
	mForwardVector = ForwardVector;
}

void TransformComponent::SetRightVector(const XMVECTOR& RightVector)
{
	mRightVector = RightVector;
}

void TransformComponent::SetUpVector(const XMVECTOR& UpVector)
{
	mUpVector = UpVector;
}

void TransformComponent::SetParentTransform(TransformComponent* parentTransform)
{
	mParentTransform = parentTransform;

	XMVECTOR det /* = XMMatrixDeterminant(parentTransform->mWorldMatrix)*/;
	mLocalMatrix = mWorldMatrix * XMMatrixInverse(&det, parentTransform->mRotationMatrix * parentTransform->mTranslationMatrix);
	
	//UpdateWorldMatrix();
}

void TransformComponent::UpdateWorldMatrix()
{
	// SRT - default order of matrix multiplication
	// (S)TR - orbit effect for Solar system could be used
	mScaleMatrix = XMMatrixScalingFromVector(mScaleVector);
	mRotationMatrix = XMMatrixRotationQuaternion(mOrientationQuat);
	mTranslationMatrix = XMMatrixTranslationFromVector(mPosVector);

	mWorldMatrix = mScaleMatrix * mRotationMatrix * mTranslationMatrix;

	if (mParentTransform) mWorldMatrix = mLocalMatrix * mParentTransform->mRotationMatrix * mParentTransform->mTranslationMatrix;
}