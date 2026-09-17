#include "stdafx.h"
#include "MovementComponent.h"
#include "Graphics/ScaldCoreTypes.h"
#include "GameFramework/Objects/Actor.h"

using namespace Scald;

MovementComponent::MovementComponent(std::shared_ptr<Actor> owner)
    : ScaldActorComponent(owner)
    , mSpeed(0.0f)
    , mJumpZVelocity(0.0f)
    , mAngle(0.0f)
    , mMovementDirection(ScaldMath::ZeroVector)
    , mOrbitAngle(0.0f)
    , mRotAngle(0.0f)
{
}

MovementComponent::~MovementComponent()
{
    
}

void MovementComponent::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
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

void MovementComponent::SetJumpZ(float JumpZ)
{
    mJumpZVelocity = JumpZ;
}