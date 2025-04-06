#include "Sphere.h"

Sphere::Sphere() : SceneGeometry()
{
}

Sphere::Sphere(const tuple<vector<VertexTex>, vector<DWORD>>& vi) : SceneGeometry()
{
	vertices = std::get<0>(vi);
	indices = std::get<1>(vi);
}

Sphere::~Sphere()
{
}

void Sphere::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& filePath, const std::wstring& texturePath)
{
	SceneGeometry::Init(pDevice, pDeviceContext);
}

void Sphere::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

