#pragma once

#include "../../Graphics/ScaldCoreTypes.h"
#include "../../ScaldCore/Engine/ScaldTimer.h"

class IScaldComponent
{
public:
	// if IScaldComponent has a number of members that should be initialized from derived
	IScaldComponent() {};

	virtual ~IScaldComponent() = 0 {};
	virtual void Update(const ScaldTimer& st) = 0;
};