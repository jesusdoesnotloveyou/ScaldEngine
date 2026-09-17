#pragma once

#include "GameFramework/Components/PrimitiveComponent.h"
#include <memory>

namespace Scald
{
    class Actor;

    class MeshComponent : public PrimitiveComponent
    {
        using Super = PrimitiveComponent;
    public:
        MeshComponent();
        MeshComponent(std::shared_ptr<Actor> owner);
        virtual ~MeshComponent() noexcept override;

        virtual void Tick(float deltaTime) override;
    };
}