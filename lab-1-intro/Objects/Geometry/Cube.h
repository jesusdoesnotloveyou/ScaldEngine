#pragma once

#include "Rectangle.h"

class Cube : public PrimitiveGeometry
{
public:
	Cube();
	Cube(const STransform& transform);

	virtual ~Cube() override;

	//~ Begin of PrimitiveGeometry interface
	virtual void Update(float DeltaTime) override;
	virtual void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext) override;
	//~ End of PrimitiveGeometry interface

private:
	void SetIndeces();
	void SetVertices();
};