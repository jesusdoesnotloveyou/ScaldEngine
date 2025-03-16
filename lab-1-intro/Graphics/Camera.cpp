#include "Camera.h"

Camera::Camera()
	:
	mPos(0.0f, 0.0f, 0.0f),
	mRot(0.0f, 0.0f, 0.0f),
	mPosVector(XMLoadFloat3(&mPos)),
	mRotVector(XMLoadFloat3(&mRot))
{
	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	const float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return mViewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return mPosVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return mPos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return mRotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return mRot;
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	// proper way to update vector's value
	mPosVector = pos;
	XMStoreFloat3(&mPos, pos);
	UpdateViewMatrix();

}

void Camera::SetPosition(float x, float y, float z)
{
	mPos = XMFLOAT3(x, y, z);
	mPosVector = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	mPosVector += pos;
	XMStoreFloat3(&mPos, mPosVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	mPos.x += x;
	mPos.y += y;
	mPos.z += z;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	mRotVector = rot;
	XMStoreFloat3(&mRot, mRotVector);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	mRot = XMFLOAT3(x, y, z);
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	mRotVector += rot;
	XMStoreFloat3(&mRot, mRotVector);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	mRot.x += x;
	mRot.y += y;
	mRot.z += z;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z);

	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, cameraRotationMatrix);

	camTarget += mPosVector;

	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, cameraRotationMatrix);

	mViewMatrix = XMMatrixLookAtLH(mPosVector, camTarget, upDir);
}