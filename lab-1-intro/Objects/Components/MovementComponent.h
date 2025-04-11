#pragma once

#include "ScaldComponent.h"

class MovementComponent : ScaldComponent
{
public:
	MovementComponent();
	virtual ~MovementComponent() noexcept override {}

	virtual void Update(const ScaldTimer& st) override {}

	void SetVelocity(const XMVECTOR& newVelocity);
	void SetVelocity(const XMFLOAT3& newVelocity);

	FORCEINLINE XMVECTOR GetVelocity() const { mVectorVelocity; }
	FORCEINLINE float GetVelocityScalar() const 
	{ 
		return sqrtf(mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y + mVelocity.z * mVelocity.z); 
	}

	void SetRotAngle(float rotAngle);
	void SetOrbitAngle(float orbitAngle);

	FORCEINLINE float GetRotAngle() const { return mRotAngle; }
	FORCEINLINE float GetOrbitAngle() const { return mOrbitAngle; }
private:
	// the angle by which an object rotates around another object's particular axis, in radians
	float mOrbitAngle;
	// the angle by which an object rotates around a particular axis, in radians
	float mRotAngle;

	XMFLOAT3 mVelocity;
	XMVECTOR mVectorVelocity;
};