#include "Rectangle.h"

Rect::Rect() : PrimitiveGeometry() // explicitly
{
	vertices =
	{
		{ DirectX::XMFLOAT4(0.2f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.2f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.2f, -0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.2f, 0.2f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};
	indeces = {
		0, 1, 2,
		1, 0, 3
	};
}

Rect::Rect(const std::vector<Vertex>& v, const std::vector<DWORD>& i)
	: PrimitiveGeometry(v, i)
{}

Rect::~Rect()
{}

void Rect::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{

	PrimitiveGeometry::Initialize(pDevice, pDeviceContext);
}