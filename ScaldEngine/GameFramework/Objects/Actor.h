#pragma once

#include "GameFramework/ScaldObject.h"
#include "GameFramework/Components/Transform.h"
#include "GameFramework/Components/SceneComponent.h"

#include <memory>

namespace Scald
{
    class World;
    class SceneComponent;

    // A base class for every object placed in the world.
    // This class contains a basic functionality to operate on the "object placed in the world".
    // Actor itself doesn't have a transform, it depends on transform of the root component
    class Actor : public ScaldObject, public std::enable_shared_from_this<Actor>
    {
        using Super = ScaldObject;
    public:
        Actor();
        virtual ~Actor() noexcept override;

        virtual void Tick(float deltaTime) override;
        virtual void OnSpawn(World* owner);
        virtual void DestroyActor();

        World* GetWorld() const;
        std::vector<std::shared_ptr<ScaldActorComponent>> GetComponents() const { return m_components; }

        // Keen on with smart pointers just to not care about object-component creating and management that much
        template<typename T, typename... Args>
        std::shared_ptr<T> CreateComponent(Args ...args)
        {
            auto component = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
            
            if (!m_rootComponent && std::is_base_of_v<SceneComponent, T>)
            {
                m_rootComponent = component;
            }
            else if (m_rootComponent)
            {
                component->SetupAttachment(m_rootComponent.get());
            }

            component->OnRegister();            // first initializer created component
            m_components.push_back(component);  // then add it to the list
            return component;
        }

        /*const FTransform& GetTransform() const { return ActorToWorld(); }
        FVector GetActorLocation() const;
        FRotator GetActorRotation() const;
        FVector GetActorScale() const;

        bool SetActorTransform(const Transform& newTransform);
        bool SetActorLocation(const FVector& newLocation);
        bool SetActorRotation(const FRotator& newRotation);
        bool SetActorScale(const FVector& newScale);*/
        
        FORCEINLINE const Transform& ActorToWorld() const
        {
            return m_rootComponent ? m_rootComponent->GetComponentTransform() : Transform::Identity;
        }

    protected:
        std::shared_ptr<SceneComponent> m_rootComponent = nullptr;
        World* ownerWorld = nullptr; // Non owning pointer
        std::vector<std::shared_ptr<ScaldActorComponent>> m_components;
    };
}  // namespace Scald