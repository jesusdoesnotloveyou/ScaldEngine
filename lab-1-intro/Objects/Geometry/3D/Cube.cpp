#include "Cube.h"

Cube::Cube() : SceneGeometry()
{
	SetVertices();
	SetIndices();
}

Cube::Cube(const tuple<vector<VertexTex>, vector<DWORD>>& vi) : SceneGeometry()
{
	vertices = std::get<0>(vi);
	indices = std::get<1>(vi);
}

Cube::~Cube()
{

}

void Cube::Init(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext, ID3D11ShaderResourceView* pTexture, const std::string& filePath)
{
	SceneGeometry::Init(mDevice, pDeviceContext, pTexture, filePath);
}

void Cube::Update(const ScaldTimer& st)
{
	SceneGeometry::Update(st);
}

void Cube::SetIndices()
{
	indices = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};
}

void Cube::SetVertices()
{
	vertices = {
		VertexTex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f),
		VertexTex(-1.0f, +1.0f, -1.0f, 1.0f, 0.0f, 0.0f),
		VertexTex(+1.0f, +1.0f, -1.0f, 1.0f, 0.0f, 0.0f),
		VertexTex(+1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f),
		VertexTex(-1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f),
		VertexTex(-1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 0.0f),
		VertexTex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 0.0f),
		VertexTex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f)
	};
}