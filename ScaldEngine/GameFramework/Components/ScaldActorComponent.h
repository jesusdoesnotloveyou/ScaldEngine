#pragma once

#include "GameFramework/ScaldObject.h"
#include <memory>

namespace Scald
{
    class AActor;
    class World;

    class ScaldActorComponent : public ScaldObject
    {
        using Super = ScaldObject;
    public:
        ScaldActorComponent();
        ScaldActorComponent(std::shared_ptr<AActor> owner);
        virtual ~ScaldActorComponent() noexcept override;

        // Supposed that components are created for actors. But if there is no actor, then ownership moves to the world.
        virtual void OnRegister();
        virtual void OnUnregister();
        virtual void Tick(float deltaTime) override;

        AActor* GetOwner() const;
        World* GetWorld() const;

    protected:
        std::weak_ptr<AActor> m_owner;
    };
}