#include "Sphere.h"

Sphere::Sphere() : SceneGeometry()
{
}

Sphere::Sphere(const tuple<vector<VertexTex>, vector<DWORD>>& vi) : SceneGeometry()
{
	vertices = std::get<0>(vi);
	indeces = std::get<1>(vi);
}

Sphere::~Sphere()
{
}

void Sphere::Init(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture)
{
	SceneGeometry::Init(mDevice, pDeviceContext, pTexture);
}

void Sphere::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

