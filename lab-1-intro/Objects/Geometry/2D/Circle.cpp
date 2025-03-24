#include "Circle.h"

#define PI 3.14159

Circle::Circle(const Transform& transform)
	: PrimitiveGeometry()
{
	radius = transform.Scale.x;

	vertices = {
		{ XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f),																	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 0 center
		{ XMFLOAT4(0.0f, radius, 0.5f, 1.0f),																XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 1 up
		{ XMFLOAT4(std::cos(ToRadians(45.f)) * radius, std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 2 45 degrees
		{ XMFLOAT4(radius, 0.0f, 0.5f, 1.0f),																XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 3 right
		{ XMFLOAT4(std::cos(ToRadians(45.f)) * radius, -std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 4 -45 degrees
		{ XMFLOAT4(0.0f, -radius, 0.5f, 1.0f),																XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 5 down
		{ XMFLOAT4(-std::cos(ToRadians(45.f)) * radius, -std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 6 -135 degrees
		{ XMFLOAT4(-radius, 0.0f, 0.5f, 1.0f),																XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 7 left
		{ XMFLOAT4(-std::cos(ToRadians(45.f)) * radius, std::sin(ToRadians(45.f)) * radius, 0.5f, 1.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 8 135 degrees
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

Circle::~Circle()
{

}

void Circle::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{
	PrimitiveGeometry::Initialize(mDevice, pDeviceContext);
}

void Circle::Update(const ScaldTimer& st)
{
	PrimitiveGeometry::Update(st);
}

float Circle::ToRadians(float angle)
{
	return float(PI) * angle / 180.f;
}
