#pragma once

#include "Graphics/ScaldCoreTypes.h"
#include "Objects/Components/SceneComponent.h"

namespace Scald
{
    class Camera : public SceneComponent
    {
        using Super = SceneComponent;
    public:
        Camera();
        virtual ~Camera() override = default;
        virtual void Update(const ScaldTimer& st) override;

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
        void SetupAttachment(TransformComponent* transformToAttach);
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