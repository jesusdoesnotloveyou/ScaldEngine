#pragma once

#include "../ScaldCore/Engine/ScaldTimer.h"

class ScaldObject
{
public:
	ScaldObject() = default;
	// it is important to make body for destructor
	virtual ~ScaldObject() noexcept = 0 {}
};