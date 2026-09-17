#include "stdafx.h"
#include "KatamariMovementComponent.h"
#include "GameFramework/Objects/Actor.h"

using namespace Scald;
using namespace DirectX;

KatamariMovementComponent::KatamariMovementComponent(std::shared_ptr<Actor> m_owner)
    : Super(m_owner)
{
    mSpeed = 10.0f;
    mAngle = 2.0f;
    mJumpZVelocity = 20.0f;
}

void KatamariMovementComponent::Tick(float deltaTime)
{
    if (XMVector3Equal(GetMovementDirection(), XMVectorZero())) return;

    UpdateMovement(deltaTime);
    UpdateRotation(deltaTime);
}

// not sure that "return's" is needed here
void KatamariMovementComponent::OnKeyPressed(unsigned char key)
{
    if (key == forwardKey)
    {
        mMovementDirection += XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        return;
    }

    if (key == backKey)
    {
        mMovementDirection += XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
        return;
    }

    if (key == rightKey)
    {
        mMovementDirection += XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
        return;
    }

    if (key == leftKey)
    {
        mMovementDirection += XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
        return;
    }
}

// not sure that "return's" is needed here
void KatamariMovementComponent::OnKeyReleased(unsigned char key)
{
    if (key == forwardKey)
    {
        mMovementDirection += XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
        return;
    }

    if (key == backKey)
    {
        mMovementDirection += XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        return;
    }

    if (key == rightKey)
    {
        mMovementDirection += XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
        return;
    }

    if (key == leftKey)
    {
        mMovementDirection += XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
        return;
    }
}

void KatamariMovementComponent::UpdateMovement(float deltaTime)
{
    const auto currentDirection = GetMovementDirection();
    //m_owner->AdjustPosition(currentDirection * mSpeed * deltaTime);
}

void KatamariMovementComponent::UpdateRotation(float deltaTime)
{
    const auto perpCurrentDirection = GetPerpendicular(std::move(GetMovementDirection()));

    XMVECTOR newRotation = XMQuaternionRotationAxis(perpCurrentDirection, XMConvertToRadians(mAngle));
    //m_owner->SetOrientation(newRotation);
}

XMVECTOR KatamariMovementComponent::GetMovementDirection() const
{
    // Normalized from PollInput in Engine.cpp
    /*const auto forward = m_owner->GetForwardVector();
    const auto right = m_owner->GetRightVector();

    XMVECTOR currentDirection = forward * XMVectorGetZ(mMovementDirection) + right * XMVectorGetX(mMovementDirection);
    currentDirection = XMVector3Normalize(currentDirection);

    return currentDirection;*/
    return XMVECTOR{};
}

// Only for XoZ plane!
XMVECTOR KatamariMovementComponent::GetPerpendicular(XMVECTOR&& vec) const
{
    auto const vecX = XMVectorGetX(vec);
    auto const vecY = XMVectorGetY(vec);
    auto const vecZ = XMVectorGetZ(vec);

    return XMVectorSet(vecZ, 0.0f, -vecX, 0.0f);
}