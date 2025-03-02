#pragma once

#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT4 posintion;
	DirectX::XMFLOAT4 color;
};

struct ConstBuffer
{
	float transformation[4][4];
};