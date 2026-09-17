#pragma once

#include "GameFramework/Components/ScaldActorComponent.h"

namespace Scald
{
    class InputComponent : public ScaldActorComponent
    {
        using Super = ScaldActorComponent;
    public:
        InputComponent() = default;
        virtual ~InputComponent() noexcept override {}

        virtual void Tick(float deltaTime) override {}
    };
}