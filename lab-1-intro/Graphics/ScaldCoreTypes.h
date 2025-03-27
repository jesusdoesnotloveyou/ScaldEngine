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

struct Transform
{
	// Object local frame
	XMFLOAT3 Scale		 = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 Rotation	 = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Translation = { 0.0f, 0.0f, 0.0f };
	
	XMMATRIX mWorldMatrix;
	XMMATRIX mLocalMatrix;

	// Solar System specific
	float orbitRot		= 0.0f; // Radians by axis
	float rot			= 0.0f;
	float orbitAngle	= 0.0f;
	float rotAngle		= 0.0f;
	float orbitRadius	= 0.0f; // by axis
	// Should be in movement component probably

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