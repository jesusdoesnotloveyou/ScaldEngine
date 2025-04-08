#pragma once

#include "Camera.h"

class SceneGeometry;

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera();

	void SetPlayerCharacter(SceneGeometry* PlayerCharacter);

private:
	SceneGeometry* mPlayerCharacter = nullptr;
};