#include "KatamariPlayer.h"

KatamariPlayer::KatamariPlayer(ModelData* modelData) : Actor(modelData)
{

}

KatamariPlayer::~KatamariPlayer() noexcept
{

}

void KatamariPlayer::Update(const ScaldTimer& st)
{
	Actor::Update(st);
}

bool KatamariPlayer::IsPlayerPawn() const
{
	return true;
}