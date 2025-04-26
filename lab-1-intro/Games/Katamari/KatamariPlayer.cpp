#include "KatamariPlayer.h"

KatamariPlayer::KatamariPlayer(ModelData* modelData) : Actor(modelData)
{
	mMovementComponent = new KatamariMovementComponent(this);
	mJumpZ = mMovementComponent->GetJumpZ();
}

KatamariPlayer::~KatamariPlayer() noexcept
{
	if (mMovementComponent) delete mMovementComponent;
}

void KatamariPlayer::Update(const ScaldTimer& st)
{
	Actor::Update(st);
	mMovementComponent->Update(st);

	if (bIsFalling)
	{
		DoJump(st);

		if (XMVectorGetY(GetPosition()) <= 1.9f)
		{
			StopJumping();
		}
	}
}

bool KatamariPlayer::IsPlayerPawn() const
{
	return true;
}

void KatamariPlayer::Jump()
{
	bIsFalling = true;
}

void KatamariPlayer::StopJumping()
{
	bIsFalling = false;
	mJumpZ = mMovementComponent->GetJumpZ();
}

void KatamariPlayer::DoJump(const ScaldTimer& st)
{	
	AdjustPosition(XMVector3Cross(GetForwardVector(), GetRightVector()) * mJumpZ * st.DeltaTime());
	mJumpZ -= 0.9f;
}