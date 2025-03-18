#include "Rectangle.h"

using namespace DirectX;

Rect::Rect() : PrimitiveGeometry()
{
	vertices =
	{
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::White) },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::White) },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::White) },
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::White) }
	};

	indeces = {
		0, 1, 2,
		0, 2, 3
	};
}

Rect::Rect(const STransform& transform)
	: Rect()
{
	ObjectTransform = transform;
	// the same logic in initialize
	pCollisionComponent->SetCenter(transform.Translation);
	pCollisionComponent->SetExtends(transform.Scale);
}

Rect::Rect(const std::vector<Vertex>& v, const std::vector<DWORD>& i)
	: PrimitiveGeometry(v, i)
{}

Rect::~Rect()
{}

void Rect::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{
	PrimitiveGeometry::Initialize(mDevice, pDeviceContext);
}

void Rect::Update(float DeltaTime)
{
	PrimitiveGeometry::Update(DeltaTime);
}