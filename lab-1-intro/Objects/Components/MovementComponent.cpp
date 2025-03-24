#include "MovementComponent.h"

void MovementComponent::Update(float DeltaTime)
{
	
}

XMFLOAT3 MovementComponent::GetVelocity() const
{
	return Velocity;
}

float MovementComponent::GetSpeed() const
{
	return mVelocity;
}

XMFLOAT3 MovementComponent::GetInitialVelocity() const
{
	return InitialVelocity;
}

XMFLOAT3 MovementComponent::GetInitialTransition() const
{
	return InitialTransition;
}

void MovementComponent::SetVelocity(const XMFLOAT3& newSpeed)
{
	Velocity = newSpeed;
}

void MovementComponent::SetVelocity(const float velocity)
{
	mVelocity = velocity;
}

void MovementComponent::SetVelocity(float x, float y, float z)
{
	Velocity.x = x;
	Velocity.y = y;
	Velocity.z = z;
}