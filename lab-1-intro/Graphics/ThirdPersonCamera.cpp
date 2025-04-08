#include "ThirdPersonCamera.h"
#include "../Objects/Components/TransformComponent.h"
#include "../Objects/Geometry/SceneGeometry.h"

ThirdPersonCamera::ThirdPersonCamera()
	: Camera() 
{}

void ThirdPersonCamera::SetPlayerCharacter(SceneGeometry* PlayerCharacter)
{
	if (mPlayerCharacter == PlayerCharacter || !PlayerCharacter) return;
	mPlayerCharacter = PlayerCharacter;

	mTransformComponent->SetParentTransform(PlayerCharacter->GetTransform());

	UpdateViewMatrix();
}