#include "Camera.h"
#include "../Objects/Components/TransformComponent.h"

Camera::Camera()
{
	UpdateViewMatrix();
}

void Camera::Update(const ScaldTimer& st)
{
	SceneComponent::Update(st);
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

void Camera::SetPosition(const XMVECTOR& pos)
{
	SceneComponent::SetPosition(pos);
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	SceneComponent::SetPosition(x, y, z);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	SceneComponent::AdjustPosition(pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	SceneComponent::AdjustPosition(x, y, z);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	SceneComponent::SetRotation(rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	SceneComponent::SetRotation(x, y, z);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	SceneComponent::AdjustRotation(rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	SceneComponent::AdjustRotation(x, y, z);
	UpdateViewMatrix();
}

void Camera::SetLookAtPosition(XMFLOAT3 lookAtPosition)
{
	// May be is would be enough to use GetPosition() from SceneComponent
	if (lookAtPosition.x == GetTransform()->GetPositionFloat3().x
		&& lookAtPosition.y == GetTransform()->GetPositionFloat3().y
		&& lookAtPosition.z == GetTransform()->GetPositionFloat3().z)
		return;

	lookAtPosition.x = GetTransform()->GetPositionFloat3().x - lookAtPosition.x;
	lookAtPosition.y = GetTransform()->GetPositionFloat3().y - lookAtPosition.y;
	lookAtPosition.z = GetTransform()->GetPositionFloat3().z - lookAtPosition.z;

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

	GetTransform()->SetRotation(pitch, yaw, 0.0f);
}

void Camera::SetLookAtPosition(XMVECTOR lookAtPosition)
{
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, lookAtPosition);
	SetLookAtPosition(tmp);
}

void Camera::SetupAttachment(TransformComponent* transformToAttach)
{
	GetTransform()->SetParentTransform(transformToAttach);
	bIsAttached = true;
}

void Camera::ClearAttachment()
{
	GetTransform()->SetParentTransform(nullptr);
	bIsAttached = false;
}

void Camera::UpdateViewMatrix()
{
	const XMVECTOR rot = GetRotation();
	const XMVECTOR pos = GetPosition();
	//Calculate camera rotation matrix
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYawFromVector(rot);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(ScaldMath::ForwardVector, cameraRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += pos;

	//Calculate forward direction based on current rotation
	const XMVECTOR forward = XMVector3TransformCoord(ScaldMath::ForwardVector, cameraRotationMatrix);
	//Calculate right direction based on current rotation
	const XMVECTOR right = XMVector3TransformCoord(ScaldMath::RightVector, cameraRotationMatrix);
	//Calculate up direction based on current rotation
	const XMVECTOR up = XMVector3TransformCoord(ScaldMath::UpVector, cameraRotationMatrix);
	
	//Rebuild view matrix
	mViewMatrix = XMMatrixLookAtLH(pos, camTarget, up);

	SetForwardVector(forward);
	SetRightVector(right);
	SetUpVector(up);
}