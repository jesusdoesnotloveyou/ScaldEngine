#include "stdafx.h"

#include "Actor.h"

#include "GameFramework/Components/Collision/CollisionComponent.h"
#include "GameFramework/World.h"

using namespace Scald;

AActor::AActor()
{
    //m_bCastsShadow = true;
}

AActor::~AActor() noexcept
{

}

void AActor::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void AActor::OnSpawn(World* owner)
{
    ownerWorld = owner;
}

void AActor::DestroyActor()
{
    // TODO: unregister all components
}

World* AActor::GetWorld() const
{
    return ownerWorld ? ownerWorld : nullptr;
}
