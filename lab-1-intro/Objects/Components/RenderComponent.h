#pragma once

#include "IScaldComponent.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class RenderComponent : public IScaldComponent
{
public:
	RenderComponent();
	virtual ~RenderComponent() noexcept override;

	// Begin of IScaldComponent interface
	virtual void Update(float DeltaTime) override;
	// End of IScaldComponent interface

	void Initialize(ID3D11Device* mDevice);
};