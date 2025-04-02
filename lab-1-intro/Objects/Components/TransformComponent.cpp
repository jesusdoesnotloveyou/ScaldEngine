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

void TransformComponent::SetLookAtPosition(XMFLOAT3 lookAtPosition)
{
	if (lookAtPosition.x == mPos.x && lookAtPosition.y == mPos.y && lookAtPosition.z == mPos.z) return;

	lookAtPosition.x = mPos.x - lookAtPosition.x;
	lookAtPosition.y = mPos.y - lookAtPosition.y;
	lookAtPosition.z = mPos.z - lookAtPosition.z;

	float pitch = 0.0f;
	if (lookAtPosition.y != 0.0f)
	{
		const float distance = static_cast<float>(sqrt(lookAtPosition.x * lookAtPosition.x + lookAtPosition.z * lookAtPosition.z));
		pitch = static_cast<float>(atan(lookAtPosition.y / distance));
	}

	float yaw = 0.0f;
	if (lookAtPosition.x != 0.0f)
	{
		yaw = static_cast<float>(atan(lookAtPosition.x / lookAtPosition.z));
	}
	if (lookAtPosition.z > 0)
		yaw += XM_PI;

	SetRotation(pitch, yaw, 0.0f);
}

void TransformComponent::SetLookAtPosition(XMVECTOR lookAtPosition)
{
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, lookAtPosition);
	SetLookAtPosition(tmp);
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

void TransformComponent::UpdateLocalMatrix()
{
	// SRT - default order of matrix multiplication
	// (S)TR - orbit effect for Solar system could be used
	mLocalMatrix =
		XMMatrixScaling(mScale.x, mScale.y, mScale.z) *
		XMMatrixRotationY(mRot.y) *
		XMMatrixTranslation(mPos.x, mPos.y, mPos.z) *
		XMMatrixRotationY(mOrbitRot);
}

void TransformComponent::UpdateWorldMatrix()
{
	UpdateLocalMatrix();

	if (ParentTransform)
	{
		XMVECTOR outScale;
		XMVECTOR outRot;
		XMVECTOR outTrans;
		XMMatrixDecompose(&outScale, &outRot, &outTrans, ParentTransform->mWorldMatrix);
		mWorldMatrix = mLocalMatrix * XMMatrixTranslationFromVector(outTrans);
	}
	else
	{
		mWorldMatrix = mLocalMatrix;
	}
}