#pragma once

#include "PrimitiveGeometry.h"

class Rect : public PrimitiveGeometry
{
public:
	Rect();

	virtual ~Rect() override;
	virtual void Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext) override;
	virtual void Update(const ScaldTimer& st) override;
};