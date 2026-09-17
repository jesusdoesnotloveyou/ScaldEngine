#pragma once

#include "Graphics/DXHelper.h"
#include "GameFramework/Components/ScaldActorComponent.h"

#include <memory>

namespace Scald
{
    using namespace DirectX;

    class Actor;

    class MovementComponent : public ScaldActorComponent
    {
        using Super = ScaldActorComponent;
    public:
        MovementComponent(std::shared_ptr<Actor> owner);
        virtual ~MovementComponent() noexcept override;

        virtual void Tick(float deltaTime) override;

        void SetRotAngle(float rotAngle);
        void SetOrbitAngle(float orbitAngle);
        void SetJumpZ(float JumpZ);

        FORCEINLINE float GetRotAngle() const { return mRotAngle; }
        FORCEINLINE float GetOrbitAngle() const { return mOrbitAngle; }
        FORCEINLINE float GetSpeed() const { return mSpeed; }
        FORCEINLINE float GetJumpZ() const { return mJumpZVelocity; }

    protected:
        float mSpeed = 0.0f;
        float mJumpZVelocity = 0.0f;
        float mAngle = 0.0f;
        XMVECTOR mMovementDirection;

        // the angle by which an object rotates around another object's particular axis, in radians
        float mOrbitAngle;
        // the angle by which an object rotates around a particular axis, in radians
        float mRotAngle;
    };
}