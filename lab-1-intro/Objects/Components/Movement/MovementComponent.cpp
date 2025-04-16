#include "MovementComponent.h"

MovementComponent::MovementComponent()
	:
	mOrbitAngle(0.0f),
	mRotAngle(0.0f),
	mVelocity(0.0f, 0.0f, 0.0f)
{
	mVectorVelocity = XMLoadFloat3(&mVelocity);
}

void MovementComponent::SetVelocity(const XMVECTOR& newVelocity)
{
	mVectorVelocity = newVelocity;
	XMStoreFloat3(&mVelocity, mVectorVelocity);
}

void MovementComponent::SetVelocity(const XMFLOAT3& newVelocity)
{
	mVelocity = newVelocity;
	mVectorVelocity = XMLoadFloat3(&mVelocity);
}

void MovementComponent::SetRotAngle(float rotAngle)
{
	if (mRotAngle == rotAngle) return;
	mRotAngle = rotAngle;
}

void MovementComponent::SetOrbitAngle(float orbitAngle)
{
	if (mOrbitAngle == orbitAngle) return;
	mOrbitAngle = orbitAngle;
}
