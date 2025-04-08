#include "ThirdPersonCamera.h"

void ThirdPersonCamera::SetPlayerCharacter(SceneGeometry* PlayerCharacter)
{
	if (mPlayerCharacter == PlayerCharacter || !PlayerCharacter) return;
	mPlayerCharacter = PlayerCharacter;

	UpdateViewMatrix();
}