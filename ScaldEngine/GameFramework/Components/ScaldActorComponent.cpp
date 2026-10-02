#include "stdafx.h"
#include "ScaldActorComponent.h"
#include "GameFramework/Objects/Actor.h"
#include "GameFramework/World.h"

using namespace Scald;

ScaldActorComponent::ScaldActorComponent() 
{

}

ScaldActorComponent::ScaldActorComponent(std::shared_ptr<Actor> owner)
    : m_owner(owner)
{
}

ScaldActorComponent::~ScaldActorComponent()
{

}

void ScaldActorComponent::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void ScaldActorComponent::OnRegister()
{

}

void ScaldActorComponent::OnUnregister()
{

}

Actor* ScaldActorComponent::GetOwner() const
{
    return m_owner.expired() ? nullptr : m_owner.lock().get();
}

World* ScaldActorComponent::GetWorld() const
{
    return GetOwner()->GetWorld();
}