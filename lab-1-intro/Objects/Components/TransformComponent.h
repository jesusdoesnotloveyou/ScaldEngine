#pragma once

#include "IScaldComponent.h"

class TransformComponent : public IScaldComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent() noexcept override {}
	virtual void Update(const ScaldTimer& st) override {}

	void SetWorldMatrix(const XMMATRIX& worldMat);
	void Reset();

	// Object local frame
	XMFLOAT3 Scale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Translation = { 0.0f, 0.0f, 0.0f };

	XMMATRIX mWorldMatrix;
	XMMATRIX mLocalMatrix;

	// Solar System specific
	float orbitRadius = 0.0f;
	// Should be in movement component probably
	float orbitRot = 0.0f;
	float rot = 0.0f;
	float orbitAngle = 0.0f;
	float rotAngle = 0.0f;

	TransformComponent* ParentTransform = nullptr;
};