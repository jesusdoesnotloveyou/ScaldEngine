#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>

using namespace DirectX;

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float w, 
		   float r, float g, float b, float a)
		:
		position(x, y, z, w),
		color(r, g, b, a)
	{}

	XMFLOAT4 position = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT4 color	  = { 1.0f, 1.0f, 1.0f, 1.0f };
};

struct VertexTex
{
	VertexTex() {}

	VertexTex(float x, float y, float z, float w,
			  float u, float v)
		:
		position(x, y, z, w),
		texCoord(u, v)
	{}

	XMFLOAT4 position = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT2 texCoord = { 0.0f, 0.0f };
};

struct ConstBuffer
{
	XMMATRIX transform = XMMatrixIdentity();
};

struct Transform
{
	// Object local frame
	XMFLOAT3 Scale		 = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 Rotation	 = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Translation = { 0.0f, 0.0f, 0.0f };
	
	XMMATRIX mWorldMatrix;
	XMMATRIX mLocalMatrix;

	// Solar System specific
	float rotationAngle	= 0.0f; // Radians by axis
	float orbitRadius	= 0.0f; // by axis
	// Should be in movement component probably
	float rotationSpeed = 0.0f; // in Radians per smth

	Transform* ParentTransform = nullptr;

	Transform()
	{
		mLocalMatrix = mWorldMatrix = XMMatrixIdentity();
	}

	void SetWorldMatrix(const XMMATRIX& worldMat)
	{
		mWorldMatrix = worldMat;
	}

	void Reset()
	{
		mLocalMatrix = mWorldMatrix = XMMatrixIdentity();
	}
};