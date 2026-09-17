#include "World.h"
#include "Graphics/Scene/Scene.h"

using namespace Scald;

World::World(Scene* scene)
{

}

World::World()
{

}

void World::DestroyActor(Actor* actor)
{

}

void World::Tick(float deltaTime)
{

}

Scene* World::GetScene() const
{
    return m_scene.get();
}