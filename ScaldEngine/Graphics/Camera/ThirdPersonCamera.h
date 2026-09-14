#pragma once

#include "Camera.h"

namespace Scald
{
    class SceneGeometry;

    class ThirdPersonCamera final : public Camera
    {
        using Super = Camera;
    public:
        ThirdPersonCamera();

    public:
        virtual void Update(const ScaldTimer& st) override;
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
        float mYaw;
        float mPitch;
        float mArmLength = 15.0f;

        std::shared_ptr<SceneComponent> m_target = nullptr;
    };
}