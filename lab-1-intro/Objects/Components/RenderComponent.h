#pragma once

#include "ScaldComponent.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class RenderComponent : public ScaldComponent
{
public:
	RenderComponent();
	virtual ~RenderComponent() noexcept override {};

	// Begin of ScaldComponent interface
	virtual void Update(const ScaldTimer& st) override;
	// End of ScaldComponent interface
};