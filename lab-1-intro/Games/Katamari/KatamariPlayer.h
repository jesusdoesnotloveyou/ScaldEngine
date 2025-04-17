#pragma once

#include "../../Objects/Geometry/Actor.h"

class KatamariPlayer : public Actor
{
public:
	KatamariPlayer() = default;
	KatamariPlayer(ModelData* modelData);
	virtual ~KatamariPlayer() noexcept override;

	virtual void Update(const ScaldTimer& st) override;
	virtual bool IsPlayerPawn() const;
};