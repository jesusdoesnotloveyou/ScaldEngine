#pragma once

#include "ScaldCore/Engine/ScaldTimer.h"
#include "Graphics/DXHelper.h"

class ScaldObject : public std::enable_shared_from_this<ScaldObject>
{
public:
	ScaldObject() = default;
	// it is important to make body for destructor
	virtual ~ScaldObject() noexcept = default;
	virtual void Update(const ScaldTimer& st) = 0;
};