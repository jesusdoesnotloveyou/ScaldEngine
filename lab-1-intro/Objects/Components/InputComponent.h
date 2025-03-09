#pragma once

#include "IScaldComponent.h"

class InputComponent : public IScaldComponent
{
public:
	InputComponent() = default;
	virtual ~InputComponent() noexcept override = default;

	virtual void Update() override;
};