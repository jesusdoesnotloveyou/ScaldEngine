#pragma once

#include "Graphics/ScaldCoreTypes.h"
#include "GameFramework/Components/SceneComponent.h"

namespace Scald
{
    class Actor;

    class Camera : public SceneComponent
    {
        using Super = SceneComponent;
    public:
        Camera() = default;
        Camera(std::shared_ptr<Actor> owner);
        virtual ~Camera() noexcept override = default;
        virtual void Tick(float deltaTime) override;

        void Reset(float fovDegrees, float aspectRatio, float nearZ, float farZ);

        const XMMATRIX& GetViewMatrix() const;
        const XMMATRIX& GetPerspectiveProjectionMatrix() const;
        const XMMATRIX& GetOrthographicProjectionMatrix() const;

        FORCEINLINE float GetFovRad() const { return m_fovYRadians; }

        void SetLookAtPosition(XMVECTOR lookAtPosition);
    private:
        void SetLookAtPosition(XMFLOAT3 lookAtPosition);
    

    private:
        FORCEINLINE float GetNearWindowHeight() const { return m_nearWindowHeight; }
        FORCEINLINE float GetNearWindowWidth() const { return m_nearWindowHeight * m_aspectRatio; }

        void SetFieldOfView();

        void UpdateView();
        void UpdatePerspectiveProjection();
        void UpdateOrthographicProjection();

    private:
        XMMATRIX m_viewMatrix;
        XMMATRIX m_perspectiveProjectionMatrix;
        XMMATRIX m_orthographicProjectionMatrix;

        float m_fovYRadians;
        float m_aspectRatio;
        float m_nearZ;
        float m_farZ;
        float m_nearWindowHeight;
        float m_farWindowHeight;

        float m_speed;
    };
}