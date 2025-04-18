#pragma once

// SCALD MACRO
#define FORCEINLINE __forceinline
//

#include <DirectXMath.h>
#include <DirectXColors.h>

using namespace DirectX;

// SCALD MATH
namespace ScaldMath
{
	const XMVECTOR ForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR RightVector	 = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR UpVector		 = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR ZeroVector	 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

// just position and color
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float w,
		float r, float g, float b, float a)
		:
		position(x, y, z, w),
		color(r, g, b, a)
	{}

	// to support direct colors
	Vertex(float x, float y, float z, float w, const XMVECTORF32 v)
		:
		position(x, y, z, w),
		color(v.f[0], v.f[1], v.f[2], v.f[3])
	{
	}

	XMFLOAT4 position = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT4 color	  = { 1.0f, 1.0f, 1.0f, 1.0f };
};

// position & texture 
struct VertexTex
{
	VertexTex() {}

	VertexTex(float x, float y, float z, float w,
		float u, float v,
		float nx, float ny, float nz)
		:
		position(x, y, z, w),
		texCoord(u, v),
		normal(nx, ny, nz)
	{}

	XMFLOAT4 position = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT2 texCoord = { 0.0f, 0.0f };
	XMFLOAT3 normal   = { 0.0f, 0.0f, 0.0f };
};

// Constant buffer types
struct ConstBufferVS
{
	XMMATRIX gWorldViewProj = XMMatrixIdentity();
	XMMATRIX gWorld = XMMatrixIdentity();
};

struct ConstBufferPS
{
	XMFLOAT3 ambientLightColor = { 0.0f, 0.0f, 0.0f };
	float ambientLightStrength = 0.0f;
	// 16 bytes
	XMFLOAT3 dynamicLightColor = { 0.0f, 0.0f, 0.0f };
	float dynamicLightStrength = 0.0f;
	// 32 bytes
	XMFLOAT3 dynamicLightPosition = { 0.0f, 0.0f, 0.0f };
	
	float attenuationConstantFactor = 1.0f;
	// 48 bytes
	float attenuationLinearFactor = 0.1f;
	float attenuationExponentialFactor = 0.05f;
	// 56 bytes
};

//struct ConstBufferPS
//{
//	float alpha = 1.0f;
//};