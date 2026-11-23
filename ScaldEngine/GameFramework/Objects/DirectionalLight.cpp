#include "DirectionalLight.h"

using namespace Scald;

ADirectionalLight::ADirectionalLight()
{
}

ADirectionalLight::~ADirectionalLight()
{
}

void ADirectionalLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADirectionalLight::OnSpawn(World* owner)
{
    Super::OnSpawn(owner);
}

void ADirectionalLight::DestroyActor()
{
    Super::DestroyActor();
}