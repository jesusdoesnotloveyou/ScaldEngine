#include "stdafx.h"
#include "PrimitiveComponent.h"
#include "GameFramework/Objects/Actor.h"
#include "Graphics/Scene/PrimitiveSceneProxy.h"

using namespace Scald;

PrimitiveComponent::PrimitiveComponent()
{

}

PrimitiveComponent::PrimitiveComponent(std::shared_ptr<Actor> owner) 
    : Super(owner)
{
}

PrimitiveComponent::~PrimitiveComponent() noexcept
{

}

void PrimitiveComponent::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void PrimitiveComponent::OnRegister()
{
    
}

void PrimitiveComponent::OnUnregister()
{

}