#include "stdafx.h"

#include "Actor.h"

#include "GameFramework/Components/Collision/CollisionComponent.h"
#include "GameFramework/World.h"

using namespace Scald;

Actor::Actor()
{
    //m_bCastsShadow = true;
}

Actor::~Actor() noexcept
{

}

void Actor::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void Actor::OnSpawn(World* owner)
{
    ownerWorld = owner;
}

void Actor::DestroyActor()
{
    // TODO: unregister all components
}

World* Actor::GetWorld() const
{
    return ownerWorld ? ownerWorld : nullptr;
}
