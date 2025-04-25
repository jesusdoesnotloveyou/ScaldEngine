#include "MovementComponent.h"
#include "../../Geometry/SceneGeometry.h"

MovementComponent::MovementComponent(SceneGeometry* CompOwner)
	:
	mSpeed(0.0f),
	mAngle(0.0f),
	mMovementDirection(ScaldMath::ZeroVector),
	mOrbitAngle(0.0f),
	mRotAngle(0.0f)
{
	Owner = CompOwner;
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