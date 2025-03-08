#pragma once

#include "PrimitiveGeometry.h"

class Rect : public PrimitiveGeometry
{
public:
	Rect();
	Rect(const std::vector<Vertex>& v, const std::vector<DWORD>& i);

	virtual ~Rect() override;
	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
};