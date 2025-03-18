#include "Star.h"

Star::Star() : PrimitiveGeometry() // explicitly
{	
	vertices = {
		// outer points
		{ DirectX::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },		// 0 up
		{ DirectX::XMFLOAT4(-0.951f, 0.309f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },	// 1 left up
		{ DirectX::XMFLOAT4(-0.588f, -0.809f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // 2 left down
		{ DirectX::XMFLOAT4(0.588f, -0.809f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 3 right down
		{ DirectX::XMFLOAT4(0.951f, 0.309f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 4 right up
		// inner points
		{ DirectX::XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },		// 5 up
		{ DirectX::XMFLOAT4(-0.475f, 0.154f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },  // 6 left up
		{ DirectX::XMFLOAT4(-0.294f, -0.404f, 0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // 7 left down
		{ DirectX::XMFLOAT4(0.294f, -0.404f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 8 right down
		{ DirectX::XMFLOAT4(0.475f, 0.154f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },	// 9 right up
	};

	indeces = {
		0, 5, 1,
		1, 6, 2,
		2, 7, 3,
		3, 8, 4,
		4, 9, 0
	};
}

Star::Star(const std::vector<Vertex>& v, const std::vector<DWORD>& i)
	: PrimitiveGeometry(v, i)
{}

Star::~Star()
{}

void Star::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{

	PrimitiveGeometry::Initialize(mDevice, pDeviceContext);
}

void Star::Update(float DeltaTime)
{
	PrimitiveGeometry::Update(DeltaTime);
}