#include "KatamariPlayer.h"

KatamariPlayer::KatamariPlayer(ModelData* modelData) : Actor(modelData)
{
	mMovementComponent = new KatamariMovementComponent(this);
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
		StopJumping();
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
}

void KatamariPlayer::DoJump(const ScaldTimer& st)
{
	float gravity = 0.5f;

}