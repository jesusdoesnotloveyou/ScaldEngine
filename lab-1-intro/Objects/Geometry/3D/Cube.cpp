#include "Cube.h"

Cube::Cube() : PrimitiveGeometry()
{
	SetVertices();
	SetIndeces();
}

Cube::Cube(const tuple<vector<Vertex>, vector<DWORD>>& vi) : PrimitiveGeometry()
{
	vertices = std::get<0>(vi);
	indeces = std::get<1>(vi);
}

Cube::~Cube()
{

}

void Cube::Update(const ScaldTimer& st)
{
	PrimitiveGeometry::Update(st);
}

void Cube::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{
	PrimitiveGeometry::Initialize(mDevice, pDeviceContext);
}

void Cube::SetIndeces()
{
	indeces = {
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
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(Colors::White) },
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1.0f), XMFLOAT4(Colors::Black) },
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f, 1.0f), XMFLOAT4(Colors::Red) },
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(Colors::Green) },
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f, 1.0f), XMFLOAT4(Colors::Blue) },
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1.0f), XMFLOAT4(Colors::Yellow) },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f, 1.0f), XMFLOAT4(Colors::Cyan) },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f, 1.0f), XMFLOAT4(Colors::Magenta) }
	};
}