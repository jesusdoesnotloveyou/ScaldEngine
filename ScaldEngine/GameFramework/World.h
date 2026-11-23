#pragma once

#include "GameFramework/Objects/Actor.h"
#include "GameFramework/Components/Light/LightComponent.h"
#include "ScaldCore/Input/InputDevice.h"

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

        template<typename T = AActor, typename... Args>
        std::shared_ptr<T> SpawnActor(Args&&... args)
        {
            assert((std::is_base_of_v<AActor, T> == true && "Your class must be derived from AActor class"));

            auto actor = std::make_shared<T>(std::forward<Args>(args)...);
            m_actors.push_back(actor);  // put an actor in the list
            actor->OnSpawn(this);       // then intiailize and register it components
            return actor;
        }

        void DestroyActor(AActor* actor);
        void Tick(float deltaTime);

        //void AddPrimitive();

        Scene* GetScene() const;

        void SendInputToControllableSceneComponent(const Keyboard::Event event, unsigned int keyCode);

    private:
        // Either player pawn root component or camera component
        std::shared_ptr<SceneComponent> m_controllableComponent;

        // Components, not objects
        std::vector<std::shared_ptr<LightComponent>> m_lights;
        // Objects
        std::vector<std::shared_ptr<AActor>>          m_actors;

        std::unique_ptr<Scene>                       m_scene;
    };
}