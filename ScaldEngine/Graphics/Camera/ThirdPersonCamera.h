#pragma once

#include "Camera.h"
#include <memory>

namespace Scald
{
    class Actor;

    class ThirdPersonCamera final : public Camera
    {
        using Super = Camera;
    public:
        ThirdPersonCamera();
        ThirdPersonCamera(std::shared_ptr<Actor> owner);

    public:
        virtual void Tick(float deltaTime) override;
        virtual void AdjustRotation(float x, float y, float z) override;

        template <typename T>
        void SetTarget(std::shared_ptr<T> playerCharacter)
        {
            static_assert(std::is_base_of<SceneComponent, T>::value, "Camera target must be a scene component!");

            if (m_target != playerCharacter && playerCharacter)
            {
                m_target = playerCharacter;
            }
            SetLookAtPosition(m_target->GetPosition());
        }

        std::shared_ptr<SceneComponent> GetTarget() const;

    private:
        void UpdatePosition();
        
    private:
        float mYaw;
        float mPitch;
        float mArmLength = 15.0f;

        std::shared_ptr<SceneComponent> m_target = nullptr;
    };
}