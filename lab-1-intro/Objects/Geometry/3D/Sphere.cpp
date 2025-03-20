#include "Sphere.h"

Sphere::Sphere() : PrimitiveGeometry()
{
}

Sphere::Sphere(const STransform& transform) : PrimitiveGeometry(transform)
{
}

Sphere::Sphere(const tuple<vector<Vertex>, vector<DWORD>>& vi) : PrimitiveGeometry()
{
	vertices = std::get<0>(vi);
	indeces = std::get<1>(vi);
}

Sphere::~Sphere()
{
}

void Sphere::Update(const ScaldTimer& st)
{
	PrimitiveGeometry::Update(st);
}

void Sphere::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{
	PrimitiveGeometry::Initialize(mDevice, pDeviceContext);
}
