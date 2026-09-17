#pragma once

#include "GameFramework/ScaldObject.h"
#include <memory>

namespace Scald
{
    class Actor;

    class ScaldActorComponent : public ScaldObject
    {
        using Super = ScaldObject;
    public:
        ScaldActorComponent();
        ScaldActorComponent(std::shared_ptr<Actor> owner);
        virtual ~ScaldActorComponent() noexcept override;

        virtual void OnRegister();
        virtual void OnUnregister();
        virtual void Tick(float deltaTime) override;

        Actor* GetOwner() const;

    protected:
        std::weak_ptr<Actor> m_owner;
    };
}