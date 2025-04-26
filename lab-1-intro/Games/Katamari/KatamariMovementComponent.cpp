#include "KatamariMovementComponent.h"
#include "../../Graphics/ScaldCoreTypes.h"
#include "../../Objects/Geometry/SceneGeometry.h"

KatamariMovementComponent::KatamariMovementComponent(SceneGeometry* OwnerPlayer) : MovementComponent(OwnerPlayer)
{
	mSpeed = 10.0f;
	mAngle = 2.0f;
	mJumpZVelocity = 20.0f;
}

void KatamariMovementComponent::Update(const ScaldTimer& st)
{
	if (XMVector3Equal(GetMovementDirection(), XMVectorZero())) return;

	UpdateMovement(st.DeltaTime());
	UpdateRotation(st.DeltaTime());
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
	Owner->AdjustPosition(currentDirection * mSpeed * deltaTime);
}

void KatamariMovementComponent::UpdateRotation(float deltaTime)
{
	const auto perpCurrentDirection = GetPerpendicular(std::move(GetMovementDirection()));

	XMVECTOR newRotation = XMQuaternionRotationAxis(perpCurrentDirection, XMConvertToRadians(mAngle));
	Owner->SetOrientation(newRotation);
}

XMVECTOR KatamariMovementComponent::GetMovementDirection() const
{
	// Normalized from PollInput in Engine.cpp
	const auto forward = Owner->GetForwardVector();
	const auto right = Owner->GetRightVector();

	XMVECTOR currentDirection = forward * XMVectorGetZ(mMovementDirection) + right * XMVectorGetX(mMovementDirection);
	currentDirection = XMVector3Normalize(currentDirection);
	
	return currentDirection;
}

// Only for XoZ plane!
XMVECTOR KatamariMovementComponent::GetPerpendicular(XMVECTOR&& vec) const
{
	auto const vecX = XMVectorGetX(vec);
	auto const vecY = XMVectorGetY(vec);
	auto const vecZ = XMVectorGetZ(vec);

	return XMVectorSet(vecZ, 0.0f, -vecX, 0.0f);
}