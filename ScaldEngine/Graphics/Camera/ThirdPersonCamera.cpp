#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "Objects/Geometry/SceneGeometry.h"

ThirdPersonCamera::ThirdPersonCamera()
{
	mPitch = 0.0f;
	mYaw = XM_PIDIV4;
}

std::shared_ptr<SceneComponent> ThirdPersonCamera::GetTarget() const
{
	return m_target;
}

void ThirdPersonCamera::Update(const ScaldTimer& st)
{
	// Update Position
	// Spherical coordinates to Cartesian
	const float NewX = XMVectorGetX(m_target->GetPosition()) + mArmLength * cosf(mPitch) * sinf(mYaw);
	const float NewY = XMVectorGetY(m_target->GetPosition()) + mArmLength * sinf(mPitch);
	const float NewZ = XMVectorGetZ(m_target->GetPosition()) + mArmLength * cosf(mPitch) * cosf(mYaw);
	SetPosition(XMVectorSet(NewX, NewY, NewZ, 0.0f));

	// Update Rotation
	SetLookAtPosition(m_target->GetPosition());
}

void ThirdPersonCamera::AdjustRotation(float x, float y, float z)
{
	mPitch += x;
	if (mPitch > XM_PIDIV2 - 0.1f) mPitch = XM_PIDIV2 - 0.1f;
	if (mPitch < -XM_PIDIV2 + 0.1f) mPitch = -XM_PIDIV2 + 0.1f;
	mYaw += y;
}

void ThirdPersonCamera::UpdateViewMatrix()
{
	const XMVECTOR rot = GetRotation();
	const XMVECTOR pos = GetPosition();
	//Calculate camera rotation matrix
	XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYawFromVector(rot);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	//XMVECTOR camTarget = mTarget->GetPosition();
	XMVECTOR camTarget = XMVector3TransformCoord(ScaldMath::ForwardVector, cameraRotationMatrix);
	//Adjust cam target to be offset by the camera's current position
	camTarget += pos;

	//Calculate forward direction based on current rotation
	const XMVECTOR forward = (XMVector3TransformCoord(ScaldMath::ForwardVector, cameraRotationMatrix));
	//Calculate right direction based on current rotation
	const XMVECTOR right = (XMVector3TransformCoord(ScaldMath::RightVector, cameraRotationMatrix));
	//Calculate up direction based on current rotation
	const XMVECTOR up = (XMVector3TransformCoord(ScaldMath::UpVector, cameraRotationMatrix));

	//Rebuild view matrix
	mViewMatrix = XMMatrixLookAtLH(pos, camTarget, up);

	SetForwardVector(forward);
	SetRightVector(right);
	SetUpVector(up);
}