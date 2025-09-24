#pragma once

#include "Objects/Components/ScaldComponent.h"

class InputComponent : public ScaldComponent
{
public:
	InputComponent() = default;
	virtual ~InputComponent() noexcept override {}

	virtual void Update(const ScaldTimer& st) override {}
};