#pragma once

#include "Graphics/ScaldCoreTypes.h"
#include "GameFramework/Components/SceneComponent.h"
#include "GameFramework/Components/Transform.h"

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

        void SetPosition(const XMVECTOR& pos) override;
        void SetPosition(float x, float y, float z) override;
        void AdjustPosition(const XMVECTOR& pos) override;
        void AdjustPosition(float x, float y, float z) override;

        void SetRotation(const XMVECTOR& rot) override;
        void SetRotation(float x, float y, float z) override;
        virtual void AdjustRotation(const XMVECTOR& rot) override;
        virtual void AdjustRotation(float x, float y, float z) override;

        void Reset(float fovDegrees, float aspectRatio, float nearZ, float farZ);

        const XMMATRIX& GetViewMatrix() const;
        const XMMATRIX& GetPerspectiveProjectionMatrix() const;
        const XMMATRIX& GetOrthographicProjectionMatrix() const;

        void SetLookAtPosition(XMVECTOR lookAtPosition);
    private:
        void SetLookAtPosition(XMFLOAT3 lookAtPosition);
    
    public:
        // camera orbital rotation specific
        void SetupAttachment(const Transform& transformToAttach);
        void ClearAttachment();
        FORCEINLINE bool IsAttached() const { return m_bIsAttached; }
        //

        FORCEINLINE float GetFovRad() const { return m_fovYRadians; }

    private:
        FORCEINLINE float GetNearWindowHeight() const { return m_nearWindowHeight; }
        FORCEINLINE float GetNearWindowWidth() const { return m_nearWindowHeight * m_aspectRatio; }

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

        bool m_bIsAttached = false;
        bool m_isDirty = false;
    };
}