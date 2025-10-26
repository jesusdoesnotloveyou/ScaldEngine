#pragma once

#include "../../Graphics/ScaldCoreTypes.h"
#include "../ScaldObject.h"

class ScaldComponent : public ScaldObject
{
public:
	// if ScaldComponent has a number of members that should be initialized from derived
	ScaldComponent() {};

	virtual ~ScaldComponent() noexcept override {};
	virtual void Update(const ScaldTimer& st) = 0; //{};
};