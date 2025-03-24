#pragma once

#include "PrimitiveGeometry.h"

class Circle : public PrimitiveGeometry
{
public:
	Circle(const Transform& transform);

	virtual ~Circle() override;
	virtual void Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext) override;
	virtual void Update(const ScaldTimer& st) override;

private:
	float ToRadians(float angle);
private:
	float radius = 0.0f;
};