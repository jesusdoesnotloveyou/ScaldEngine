#pragma once

#include "GameFramework/ScaldObject.h"
#include "GameFramework/Components/Transform.h"
#include "GameFramework/Components/SceneComponent.h"

#include <memory>
#include <string>

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

        template<typename T, typename... Args>
        std::shared_ptr<T> CreateComponent(Args ...args)
        {
            auto newComponent = std::make_shared<T>(shared_from_this(), args...);
            newComponent->AttachToParent(m_rootComponent);
            return newComponent;
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
        SceneComponent* m_rootComponent = nullptr;
    };
}  // namespace Scald