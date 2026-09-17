#include "stdafx.h"

#include "Actor.h"

#include "GameFramework/Components/Collision/CollisionComponent.h"
#include "GameFramework/Components/Transform.h"
#include "GameFramework/Components/SceneComponent.h"

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

void Actor::OnSpawn(World* owner) {}
