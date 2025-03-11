#include "MovementComponent.h"

void MovementComponent::Update(float DeltaTime)
{
	
}

float MovementComponent::GetSpeed() const
{
	return static_cast<float>(sqrt(pow(Velocity.x, 2) + pow(Velocity.y, 2) + pow(Velocity.z, 2)));
}

void MovementComponent::SetSpeed(dx::XMFLOAT3 newSpeed)
{
	Velocity = newSpeed;
}
