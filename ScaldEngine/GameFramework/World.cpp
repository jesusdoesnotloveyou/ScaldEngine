#include "World.h"
#include "Graphics/Scene/Scene.h"
#include "GameFramework/Components/PrimitiveComponent.h"

using namespace Scald;

World::World(Scene* scene)
{

}

World::World()
    : m_scene(std::make_unique<Scene>())
{

}

void World::DestroyActor(Actor* actor)
{
    for (auto& activeActor : m_actors)
    {
        activeActor->DestroyActor();
    }
}

void World::Tick(float deltaTime)
{
    // Update all active actors and their components
    for (auto& actor : m_actors)
    {
        actor->Tick(deltaTime);
        
        for (auto& component : actor->GetComponents())
        {
            // only renderable components so leave it for now
            auto primitive = dynamic_cast<PrimitiveComponent*>(component.get());

            // TODO: force update right now
            if (primitive /*&& primitive->IsDirty()*/)
            {
                // push component's world matrix to proxy's copy
                m_scene->UpdatePrimitiveTransform(primitive->GetSceneProxy(),
                    primitive->GetComponentTransform()); // SceneComponent computes this as being scene component
                primitive->ClearDirty();
            }
        }
    }
}

//void World::AddPrimitive()
//{
//    m_scene->AddPrimitive();
//}

Scene* World::GetScene() const
{
    return m_scene.get();
}