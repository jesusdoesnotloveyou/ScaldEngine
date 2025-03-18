#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT4 posintion;
	XMFLOAT4 color;
};

struct ConstBuffer
{
	XMMATRIX transform = XMMatrixIdentity();
};

struct STransform
{
	XMFLOAT3 Scale		 = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 Rotation	 = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Translation = { 0.0f, 0.0f, 0.0f };

	STransform(
		const XMFLOAT3& scale = { 1.0f, 1.0f, 1.0f },
		const XMFLOAT3& rotation = { 0.0f, 0.0f, 0.0f },
		const XMFLOAT3& translation = { 0.0f, 0.0f, 0.0f }
	) 
		: 
		Scale(scale), 
		Rotation(rotation), 
		Translation(translation)
		{}
};