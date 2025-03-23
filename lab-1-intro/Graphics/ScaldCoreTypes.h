#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT4 position	= { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT4 color		= { 1.0f, 1.0f, 1.0f, 1.0f };
};

struct ConstBuffer
{
	XMMATRIX transform = XMMatrixIdentity();
};

struct STransform
{
	// Object local frame
	XMFLOAT3 Scale		 = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 Rotation	 = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Translation = { 0.0f, 0.0f, 0.0f };

	// Solar System specific
	float rotationAngle	= 0.0f; // Radians by axis
	float orbitAngle	= 0.0f; // Radians by axis
	
	float orbitRadius	= 0.0f; // by axis

	// Should be in movement component probably
	float rotationSpeed = 0.0f; // in Radians per smth
	float orbitSpeed	= 0.0f; // in Radians per smth

	STransform* ParentTransform = nullptr;

	STransform(
		const XMFLOAT3& scale		= { 1.0f, 1.0f, 1.0f },
		const XMFLOAT3& rotation	= { 0.0f, 0.0f, 0.0f },
		const XMFLOAT3& translation = { 0.0f, 0.0f, 0.0f }
	) 
		: 
		Scale(scale), 
		Rotation(rotation), 
		Translation(translation)
		{}
};