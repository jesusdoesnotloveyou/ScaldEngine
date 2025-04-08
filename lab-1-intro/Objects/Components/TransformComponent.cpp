#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	mLocalMatrix = mWorldMatrix = XMMatrixIdentity();
	mScale = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mScaleVector = XMLoadFloat3(&mScale);
	mRotVector = XMLoadFloat3(&mRot);
	mPosVector = XMLoadFloat3(&mPos);
	mOrbitRot = 0.0f;
}

void TransformComponent::Update(const ScaldTimer& st)
{
	UpdateWorldMatrix();
}

void TransformComponent::SetWorldMatrix(const XMMATRIX& worldMat)
{
	mWorldMatrix = worldMat;
}

void TransformComponent::Reset()
{
	mLocalMatrix = mWorldMatrix = XMMatrixIdentity();
}

const XMVECTOR& TransformComponent::GetPositionVector() const
{
	return mPosVector;
}

const XMFLOAT3& TransformComponent::GetPositionFloat3() const
{
	return mPos;
}

const XMVECTOR& TransformComponent::GetRotationVector() const
{
	return mRotVector;
}

const XMFLOAT3& TransformComponent::GetRotationFloat3() const
{
	return mRot;
}

void TransformComponent::SetScale(const XMVECTOR& scaleVector)
{
	XMStoreFloat3(&mScale, scaleVector);
	mScaleVector = scaleVector;
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

void TransformComponent::SetPosition(const XMVECTOR& posVector)
{
	XMStoreFloat3(&mPos, posVector);
	mPosVector = posVector;
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

void TransformComponent::SetRotation(const XMVECTOR& rotVector)
{
	mRotVector = rotVector;
	XMStoreFloat3(&mRot, rotVector);
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

XMVECTOR TransformComponent::GetBackVector()const
{
	return mBackVector;
}

XMVECTOR TransformComponent::GetLeftVector()const
{
	return mLeftVector;
}

void TransformComponent::SetParentTransform(TransformComponent* parentTransform)
{
	mParentTransform = parentTransform;
}

void TransformComponent::UpdateLocalMatrix()
{
	// SRT - default order of matrix multiplication
	// (S)TR - orbit effect for Solar system could be used
	mLocalMatrix =
		XMMatrixScalingFromVector(mScaleVector) *
		XMMatrixRotationRollPitchYawFromVector(mRotVector) *
		XMMatrixTranslationFromVector(mPosVector) *
		XMMatrixRotationY(mOrbitRot);
}

void TransformComponent::UpdateWorldMatrix()
{
	UpdateLocalMatrix();

	if (mParentTransform)
	{
		XMVECTOR outScale;
		XMVECTOR outRot;
		XMVECTOR outTrans;
		XMMatrixDecompose(&outScale, &outRot, &outTrans, mParentTransform->mWorldMatrix);
		mWorldMatrix = mLocalMatrix * XMMatrixTranslationFromVector(outTrans);
	}
	else
	{
		mWorldMatrix = mLocalMatrix;
	}
}