#pragma once

#include "../2D/PrimitiveGeometry.h"

#include <vector>
#include <tuple>

using std::vector;
using std::tuple;

class Sphere : public PrimitiveGeometry
{
public:
	Sphere();
	Sphere(const tuple<vector<Vertex>, vector<DWORD>>& vi);
	virtual ~Sphere() override;

	//~ Begin of PrimitiveGeometry interface
	virtual void Update(const ScaldTimer& st) override;
	virtual void Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext) override;
	//~ End of PrimitiveGeometry interface

private:

};