#pragma once

#include "PrimitiveGeometry.h"

class Rect : public PrimitiveGeometry
{
public:
	Rect();
	Rect(const STransform& transform);
	Rect(const std::vector<Vertex>& v, const std::vector<DWORD>& i = { 0, 1, 2,  0, 2, 3 });

	virtual ~Rect() override;
	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
	virtual void Update(float DeltaTime) override;
};