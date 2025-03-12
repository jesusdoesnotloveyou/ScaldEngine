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

struct STransform
{
	dx::XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
	dx::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };
	dx::XMFLOAT3 Scale = { 0.0f, 0.0f, 0.0f };		// real scale in coordinate divided by two ([0,x], [0,y], [0, z])
	dx::XMFLOAT3 Center = { 0.0f, 0.0f, 0.0f };
};