#pragma once

#include "IScaldComponent.h"

class InputComponent : public IScaldComponent
{
public:
	InputComponent() = default;
	virtual ~InputComponent() noexcept override {}

	virtual void Update(const ScaldTimer& st) override {}
};