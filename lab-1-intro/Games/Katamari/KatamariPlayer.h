#pragma once

#include "../../Objects/Geometry/Actor.h"
#include "KatamariMovementComponent.h"

class KatamariPlayer : public Actor
{
public:
	KatamariPlayer() = default;
	KatamariPlayer(ModelData* modelData);
	virtual ~KatamariPlayer() noexcept override;

	virtual void Update(const ScaldTimer& st) override;
	virtual bool IsPlayerPawn() const;

public:

	void Jump();
	void StopJumping();
	void DoJump(const ScaldTimer& st);

	FORCEINLINE bool IsFalling() { return bIsFalling; }

	FORCEINLINE KatamariMovementComponent* GetMovement()const { return mMovementComponent; }

protected:
	KatamariMovementComponent* mMovementComponent = nullptr;

private:
	bool bIsFalling = false;
};