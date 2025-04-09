#include "Camera.h"
#include "../Objects/Components/TransformComponent.h"

Camera::Camera()
{
	mTransformComponent = new TransformComponent();
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

XMVECTOR Camera::GetPosition() const
{
	return mTransformComponent->GetPositionVector();
}

XMVECTOR Camera::GetRotation() const
{
	return mTransformComponent->GetRotationVector();
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	mTransformComponent->SetPosition(pos);
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	mTransformComponent->SetPosition(x, y, z);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	mTransformComponent->AdjustPosition(pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	mTransformComponent->AdjustPosition(x, y, z);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	mTransformComponent->SetRotation(rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	mTransformComponent->SetRotation(x, y, z);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	mTransformComponent->AdjustRotation(rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	mTransformComponent->AdjustRotation(x, y, z);
	UpdateViewMatrix();
}

void Camera::SetLookAtPosition(XMFLOAT3 lookAtPosition)
{
	if (lookAtPosition.x == mTransformComponent->GetPositionFloat3().x
		&& lookAtPosition.y == mTransformComponent->GetPositionFloat3().y
		&& lookAtPosition.z == mTransformComponent->GetPositionFloat3().z)
		return;

	lookAtPosition.x = mTransformComponent->GetPositionFloat3().x - lookAtPosition.x;
	lookAtPosition.y = mTransformComponent->GetPositionFloat3().y - lookAtPosition.y;
	lookAtPosition.z = mTransformComponent->GetPositionFloat3().z - lookAtPosition.z;

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

	mTransformComponent->SetRotation(pitch, yaw, 0.0f);
}

void Camera::SetLookAtPosition(XMVECTOR lookAtPosition)
{
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, lookAtPosition);
	SetLookAtPosition(tmp);
}

XMVECTOR Camera::GetForwardVector()const
{
	return mTransformComponent->GetForwardVector();
}

XMVECTOR Camera::GetRightVector()const
{
	return mTransformComponent->GetRightVector();
}

XMVECTOR Camera::GetUpVector()const
{
	return mTransformComponent->GetUpVector();
}

void Camera::SetForwardVector(const XMVECTOR& relativeForwardVector)
{
	mTransformComponent->SetForwardVector(relativeForwardVector);
}

void Camera::SetRightVector(const XMVECTOR& relativeRightVector)
{
	mTransformComponent->SetRightVector(relativeRightVector);
}

void Camera::SetUpVector(const XMVECTOR& relativeUpVector)
{
	mTransformComponent->SetUpVector(relativeUpVector);
}

void Camera::SetupAttachment(TransformComponent* transformToAttach)
{
	mTransformComponent->SetParentTransform(transformToAttach);
	bIsAttached = true;
}

void Camera::ClearAttachment()
{
	mTransformComponent->SetParentTransform(nullptr);
	bIsAttached = false;
}

void Camera::UpdateViewMatrix()
{
	const auto rot = GetRotation();
	const auto pos = GetPosition();
	//Calculate camera rotation matrix
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYawFromVector(rot);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(ScaldMath::ForwardVector, cameraRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += pos;
	//Calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(ScaldMath::UpVector, cameraRotationMatrix);
	//Rebuild view matrix
	mViewMatrix = XMMatrixLookAtLH(pos, camTarget, upDir);

	const auto posY = XMVectorGetY(pos);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, posY, 0.0f);

	const auto forward = XMVector3TransformCoord(ScaldMath::ForwardVector, vecRotationMatrix);
	const auto right = XMVector3TransformCoord(ScaldMath::RightVector, vecRotationMatrix);
	const auto up = XMVector3TransformCoord(ScaldMath::UpVector, vecRotationMatrix);

	SetForwardVector(forward);
	SetRightVector(right);
	SetUpVector(up);
}