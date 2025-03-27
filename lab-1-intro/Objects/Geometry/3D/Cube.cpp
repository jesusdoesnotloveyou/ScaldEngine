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
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, Colors::White),
		Vertex(-1.0f, +1.0f, -1.0f, 1.0f, Colors::Black),
		Vertex(+1.0f, +1.0f, -1.0f, 1.0f, Colors::Red),
		Vertex(+1.0f, -1.0f, -1.0f, 1.0f, Colors::Green),
		Vertex(-1.0f, -1.0f, +1.0f, 1.0f, Colors::Blue),
		Vertex(-1.0f, +1.0f, +1.0f, 1.0f, Colors::Yellow),
		Vertex(+1.0f, +1.0f, +1.0f, 1.0f, Colors::Cyan),
		Vertex(+1.0f, -1.0f, +1.0f, 1.0f, Colors::Magenta)
	};
}