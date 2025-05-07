#pragma once

#include "Camera.h"

class SceneGeometry;

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera();

public:
	virtual void Update(const ScaldTimer& st) override;

	virtual void AdjustRotation(float x, float y, float z) override;

	void SetTarget(SceneGeometry* PlayerCharacter);
	SceneGeometry* GetTarget() const;

protected:
	virtual void UpdateViewMatrix() override;

private:
	float mYaw;
	float mPitch;
	float mArmLength = 15.0f;

	SceneGeometry* mTarget = nullptr;
};