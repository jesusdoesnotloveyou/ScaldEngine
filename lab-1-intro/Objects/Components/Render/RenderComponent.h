#pragma once

#include "Graphics/DXHelper.h"
#include "Objects/Components/ScaldComponent.h"

class RenderComponent : public ScaldComponent
{
public:
	RenderComponent();
	virtual ~RenderComponent() noexcept override {};

	// Begin of ScaldComponent interface
	virtual void Update(const ScaldTimer& st) override;
	// End of ScaldComponent interface
};