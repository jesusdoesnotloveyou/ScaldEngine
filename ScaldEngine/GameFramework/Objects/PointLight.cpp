#include "PointLight.h"

using namespace Scald;

APointLight::APointLight()
{
}

APointLight::~APointLight()
{
}

void APointLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APointLight::OnSpawn(World* owner)
{
    Super::OnSpawn(owner);
}

void APointLight::DestroyActor()
{
    Super::DestroyActor();
}