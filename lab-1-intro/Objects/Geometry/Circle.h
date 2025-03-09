#pragma once

#include "PrimitiveGeometry.h"

class Circle : public PrimitiveGeometry
{
public:
	Circle();
	Circle(const std::vector<Vertex>& v, const std::vector<DWORD>& i);

	virtual ~Circle() override;
	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
};