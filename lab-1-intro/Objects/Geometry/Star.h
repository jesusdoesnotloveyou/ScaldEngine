#pragma once

#include "PrimitiveGeometry.h"

class Star : public PrimitiveGeometry
{
public:
	Star();

	virtual ~Star() override;
	virtual void Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext) override;
	virtual void Update(float DeltaTime) override;
};