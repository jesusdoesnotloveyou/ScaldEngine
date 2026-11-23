#include "stdafx.h"
#include "MeshComponent.h"

using namespace Scald;

MeshComponent::MeshComponent() {}

MeshComponent::MeshComponent(std::shared_ptr<AActor> owner)
    : Super(owner) {}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Tick(float deltaTime) 
{
    Super::Tick(deltaTime);
}
