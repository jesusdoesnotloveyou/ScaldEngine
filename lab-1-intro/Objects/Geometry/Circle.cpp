#include "Circle.h"

#define PI 3.14159

Circle::Circle(const STransform& transform)
	: PrimitiveGeometry()
{
	radius = transform.Scale.x;

	vertices = {
		{ DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),																DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 0 center
		{ DirectX::XMFLOAT4(0.0f, radius, 0.5f, 1.0f),																DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 1 up
		{ DirectX::XMFLOAT4(std::cos(ToRadians(45.f)) * radius, std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 2 45 degrees
		{ DirectX::XMFLOAT4(radius, 0.0f, 0.5f, 1.0f),																DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 3 right
		{ DirectX::XMFLOAT4(std::cos(ToRadians(45.f)) * radius, -std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 4 -45 degrees
		{ DirectX::XMFLOAT4(0.0f, -radius, 0.5f, 1.0f),																DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 5 down
		{ DirectX::XMFLOAT4(-std::cos(ToRadians(45.f)) * radius, -std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 6 -135 degrees
		{ DirectX::XMFLOAT4(-radius, 0.0f, 0.5f, 1.0f),																DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 7 left
		{ DirectX::XMFLOAT4(-std::cos(ToRadians(45.f)) * radius, std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 8 135 degrees
	};

	indeces = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 1
	};
}

Circle::Circle(const std::vector<Vertex>& v, const std::vector<DWORD>& i)
	: PrimitiveGeometry(v, i)
{}

Circle::~Circle()
{

}

void Circle::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	PrimitiveGeometry::Initialize(pDevice, pDeviceContext);
}

void Circle::Update(float DeltaTime)
{
	ObjectTransform.Translation.x = GetMovementComponent()->GetVelocity().x * DeltaTime * 10.0f;
	ObjectTransform.Translation.y = GetMovementComponent()->GetVelocity().y * DeltaTime * 10.0f;

	PrimitiveGeometry::Update(DeltaTime);
}

float Circle::ToRadians(float angle)
{
	return float(PI) * angle / 180.f;
}
