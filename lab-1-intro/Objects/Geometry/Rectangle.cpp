#include "Rectangle.h"

Rect::Rect(float x, float y) : PrimitiveGeometry()
{
	vertices =
	{
		{ DirectX::XMFLOAT4(-x, y, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(x, y, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(x, -y, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-x, -y, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	};
	indeces = {
		0, 1, 2,
		0, 2, 3
	};
}

Rect::Rect(const STransform& transform) 
	: PrimitiveGeometry()
{
	ObjectTransform = transform;
	// the same logic in initialize
	pCollisionComponent->SetCenter(transform.Position);
	pCollisionComponent->SetExtends(transform.Scale);

	vertices =
	{
		{ DirectX::XMFLOAT4(-transform.Scale.x + transform.Position.x, transform.Scale.y, transform.Scale.z, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(transform.Scale.x + transform.Position.x, transform.Scale.y, transform.Scale.z, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(transform.Scale.x + transform.Position.x, -transform.Scale.y, transform.Scale.z, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-transform.Scale.x + transform.Position.x, -transform.Scale.y, transform.Scale.z, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	};
	indeces = {
		0, 1, 2,
		0, 2, 3
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

void Rect::Update(float DeltaTime)
{
	PrimitiveGeometry::Update(DeltaTime);
}