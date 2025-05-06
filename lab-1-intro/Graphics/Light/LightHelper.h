#pragma once

#include "../../ScaldCore/Engine/ScaldWindows.h"
#include <DirectXMath.h>

using namespace DirectX;

enum class ELightType : uint8_t
{
	None = 0,
	Point,
	Spot,
	Directional,
	MAX = 4
};

struct DirectionalLightParams
{
	DirectionalLightParams() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient	= { 0.0f, 0.0f, 0.0f, 0.0f };			// 16 bytes
	XMFLOAT4 diffuse	= { 0.0f, 0.0f, 0.0f, 0.0f };			// 16 bytes
	XMFLOAT4 specular	= { 0.0f, 0.0f, 0.0f, 0.0f };			// 16 bytes
	XMFLOAT3 direction	= { 0.57735f, -0.57735f, 0.57735f };	// 12 bytes
	float pad = 0.0f;											// 4 bytes
																// 64 bytes
};

struct PointLightParams
{
	PointLightParams() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient	= { 0.0f, 0.0f, 0.0f, 0.0f };	// 16 bytes
	XMFLOAT4 diffuse	= { 0.0f, 0.0f, 0.0f, 0.0f };	// 16 bytes
	XMFLOAT4 specular	= { 0.0f, 0.0f, 0.0f, 0.0f };	// 16 bytes
	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };			// 12 bytes
	float range = 0.0f;
	// Packed into 4D vector: (A0, A1, A2, pad)
	XMFLOAT3 attenuation = { 0.0f, 0.0f, 0.0f };
	float pad = 0.0f;
														// 80 bytes
};

struct SpotLightParams
{
	SpotLightParams() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient	= { 0.0f, 0.0f, 0.0f, 0.0f };
	XMFLOAT4 diffuse	= { 0.0f, 0.0f, 0.0f, 0.0f };
	XMFLOAT4 specular	= { 0.0f, 0.0f, 0.0f, 0.0f };
	
	// Packed into 4D vector: (position, range)
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	float range = 0.0f;

	// Packed into 4D vector: (direction, spot)
	XMFLOAT3 direction = { 0.0f, 0.0f, 0.0f };
	float spot = 0.0f;
	
	// Packed into 4D vector: (att, pad)
	XMFLOAT3 attenuation = { 0.0f, 0.0f, 0.0f };
	float pad = 0.0f;
														// 96 bytes
};