#pragma once

#include "PrimitiveGeometry.h"

class Star : public PrimitiveGeometry
{
public:
	Star();
	Star(const std::vector<Vertex>& v, const std::vector<DWORD>& i);

	virtual ~Star() override;
	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
};