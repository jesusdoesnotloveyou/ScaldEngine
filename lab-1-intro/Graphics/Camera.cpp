#include "Camera.h"
#include "ScaldCoreTypes.h"

Camera::Camera()
{
	mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mPosVector = XMLoadFloat3(&mPos);
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	const float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	mProjectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
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

void Camera::SetPosition(const XMVECTOR& posVector)
{
	// proper way to update vector's value
	XMStoreFloat3(&mPos, posVector);
	mPosVector = posVector;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	mPos = XMFLOAT3(x, y, z);
	mPosVector = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& posVector)
{
	mPosVector += posVector;
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

void Camera::SetRotation(const XMVECTOR& rotVector)
{
	mRotVector = rotVector;
	XMStoreFloat3(&mRot, rotVector);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	mRot = XMFLOAT3(x, y, z);
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rotVector)
{
	mRotVector += rotVector;
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

void Camera::SetLookAtPosition(XMFLOAT3 lookAtPosition)
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

const XMVECTOR& Camera::GetForwardVector()
{
	return mForwardVector;
}

const XMVECTOR& Camera::GetRightVector()
{
	return mRightVector;
}

const XMVECTOR& Camera::GetBackwardVector()
{
	return mBackwardVector;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return mLeftVector;
}

void Camera::SetupAttachment(Transform* transformToAttach)
{
	mAttachmentTransform = transformToAttach;
	bIsAttached = true;
}

void Camera::UpdateViewMatrix()
{
	//Calculate camera rotation matrix
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, cameraRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += mPosVector;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, cameraRotationMatrix);
	//Rebuild view matrix
	mViewMatrix = XMMatrixLookAtLH(mPosVector, camTarget, upDir);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, mRot.y, 0.0f);
	mForwardVector = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	mBackwardVector = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	mLeftVector = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	mRightVector = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}