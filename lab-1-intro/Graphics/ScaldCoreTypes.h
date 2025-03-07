#pragma once

#include <DirectXMath.h>

namespace dx = DirectX;

struct Vertex
{
	dx::XMFLOAT4 posintion;
	dx::XMFLOAT4 color;
};

struct ConstBuffer
{
	dx::XMMATRIX transform;
};