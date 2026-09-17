#pragma once

#include "GameFramework/Components/Movement/MovementComponent.h"
#include "Graphics/ScaldCoreTypes.h"
#include "ScaldCore/Engine/ScaldTimer.h"

#include <memory>

namespace Scald
{
    class Actor;

    class KatamariMovementComponent : public MovementComponent
    {
        using Super = MovementComponent;
    public:
        KatamariMovementComponent(std::shared_ptr<Actor> owner);
        virtual ~KatamariMovementComponent() noexcept override = default;

        virtual void Tick(float deltaTime) override;

    private:
        KatamariMovementComponent(const KatamariMovementComponent&) = default;
        KatamariMovementComponent& operator=(const KatamariMovementComponent&) = default;

    public:
        void OnKeyPressed(unsigned char key);
        void OnKeyReleased(unsigned char key);

    private:
        void UpdateMovement(float deltaTime);
        void UpdateRotation(float deltaTime);
        XMVECTOR GetMovementDirection() const;
        XMVECTOR GetPerpendicular(XMVECTOR&& vec) const;

    private:
        const unsigned char forwardKey = 'W';
        const unsigned char backKey = 'S';
        const unsigned char rightKey = 'D';
        const unsigned char leftKey = 'A';
    };
}