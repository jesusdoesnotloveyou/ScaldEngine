#pragma once

#include "Camera.h"

class SceneGeometry;

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera() = default;

	void SetPlayerCharacter(SceneGeometry* PlayerCharacter);

private:
	SceneGeometry* mPlayerCharacter = nullptr;
};