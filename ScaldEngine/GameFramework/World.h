#pragma once

#include "GameFramework/Objects/Actor.h"

#include <vector>
#include <memory>
#include <cassert>
#include <type_traits>

namespace Scald
{
    class Scene;

    // World is game thread Scene class. It knows nothing about render thread.
    // It has one way relationship with FSceneProxy which it pushes changes to.
    class World
    {
    public:

        // Receives existing scene
        explicit World(Scene* scene);
        // Creates default scene
        World();

        template<typename T = Actor, typename... Args>
        T* SpawnActor(Args&&... args)
        {
            assert((std::is_base_of_v<Actor, T> == true && "Your class must be derived from Actor class"));
            
            auto actor = std::make_unique<T>(std::forward<Args>(args)...);
            auto* ptr  = actor.get();
            ptr->OnSpawn(this);               // actor registers its components
            m_actors.push_back(std::move(actor));
            return ptr;
        }

        void DestroyActor(Actor* actor);
        void Tick(float deltaTime);

        Scene* GetScene() const;

    private:
        std::vector<std::unique_ptr<Actor>> m_actors;
        std::unique_ptr<Scene>              m_scene;
    };
}