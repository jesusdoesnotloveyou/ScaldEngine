#pragma once

#include "../ScaldComponent.h"

class SceneGeometry;

class MovementComponent : ScaldComponent
{
public:
	MovementComponent(SceneGeometry* CompOwner);
	virtual ~MovementComponent() noexcept override {}

	virtual void Update(const ScaldTimer& st) override {}

	void SetRotAngle(float rotAngle);
	void SetOrbitAngle(float orbitAngle);
	void SetJumpZ(float JumpZ);

	FORCEINLINE float GetRotAngle()const { return mRotAngle; }
	FORCEINLINE float GetOrbitAngle()const { return mOrbitAngle; }
	FORCEINLINE float GetSpeed()const { return mSpeed; }
	FORCEINLINE float GetJumpZ()const { return mJumpZVelocity; }

protected:
	float mSpeed = 0.0f;
	float mJumpZVelocity = 0.0f;
	float mAngle = 0.0f;
	XMVECTOR mMovementDirection;

	// the angle by which an object rotates around another object's particular axis, in radians
	float mOrbitAngle;
	// the angle by which an object rotates around a particular axis, in radians
	float mRotAngle;

	SceneGeometry* Owner = nullptr;
};