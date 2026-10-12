#include "stdafx.h"
#include "Camera.h"
#include "GameFramework/Objects/Actor.h"

using namespace Scald;

Camera::Camera(std::shared_ptr<Actor> owner) 
    : Super(owner)
    , m_viewMatrix(XMMatrixIdentity())
    , m_perspectiveProjectionMatrix(XMMatrixIdentity())
    , m_orthographicProjectionMatrix(XMMatrixIdentity())
{
    m_bIsDirty = true; // to update view matrix on the first update call
}

void Camera::Tick(float deltaTime)
{
    // Super::Tick(deltaTime);

    if (!m_bIsDirty) return;
    UpdateView();
}

void Camera::Reset(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    m_fovYRadians = (fovDegrees / 360.0f) * XM_2PI;
    m_aspectRatio = aspectRatio;
    m_nearZ = nearZ;
    m_farZ = farZ;

    m_nearWindowHeight = 2.0f * tanf(0.5f * m_fovYRadians) * m_nearZ;
    m_farWindowHeight = 2.0f * tanf(0.5f * m_fovYRadians) * m_farZ;

    UpdatePerspectiveProjection();
    UpdateOrthographicProjection();
}

const XMMATRIX& Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const XMMATRIX& Camera::GetPerspectiveProjectionMatrix() const
{
    return m_perspectiveProjectionMatrix;
}

const XMMATRIX& Camera::GetOrthographicProjectionMatrix() const
{
    return m_orthographicProjectionMatrix;
}

void Camera::SetLookAtPosition(XMFLOAT3 lookAtPosition)
{
    const XMFLOAT3 posFloat = GetPositionFloat();
    // May be is would be enough to use GetPosition() from Super
    if (lookAtPosition.x == posFloat.x && lookAtPosition.y == posFloat.y && lookAtPosition.z == posFloat.z) return;

    lookAtPosition.x = posFloat.x - lookAtPosition.x;
    lookAtPosition.y = posFloat.y - lookAtPosition.y;
    lookAtPosition.z = posFloat.z - lookAtPosition.z;

    float pitch = 0.0f;
    if (lookAtPosition.y != 0.0f)
    {
        const float distance = static_cast<float>(sqrt(lookAtPosition.x * lookAtPosition.x + lookAtPosition.z * lookAtPosition.z));
        pitch = static_cast<float>(atan(lookAtPosition.y / distance));
    }

    float yaw = 0.0f;
    if (lookAtPosition.x != 0.0f)
    {
        yaw = static_cast<float>(atan(lookAtPosition.x / lookAtPosition.z));
    }
    if (lookAtPosition.z > 0) yaw += XM_PI;

    SetRotation(pitch, yaw, 0.0f);
}

void Camera::SetLookAtPosition(XMVECTOR lookAtPosition)
{
    XMFLOAT3 tmp;
    XMStoreFloat3(&tmp, lookAtPosition);
    SetLookAtPosition(tmp);
}

void Camera::UpdateView()
{
    const XMVECTOR rot = GetRotation();
    const XMVECTOR pos = GetPosition();
    // Calculate camera rotation matrix
    XMMATRIX cameraRotationMatrix = XMMatrixRotationRollPitchYawFromVector(rot);
    // Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
    XMVECTOR camTarget = XMVector3TransformCoord(ScaldMath::ForwardVector, cameraRotationMatrix);
    // Adjust cam target to be offset by the camera's current position
    camTarget += pos;

    // Calculate forward direction based on current rotation
    const XMVECTOR forward = XMVector3TransformCoord(ScaldMath::ForwardVector, cameraRotationMatrix);
    // Calculate right direction based on current rotation
    const XMVECTOR right = XMVector3TransformCoord(ScaldMath::RightVector, cameraRotationMatrix);
    // Calculate up direction based on current rotation
    const XMVECTOR up = XMVector3TransformCoord(ScaldMath::UpVector, cameraRotationMatrix);

    // Rebuild view matrix
    m_viewMatrix = XMMatrixLookAtLH(pos, camTarget, up);

    // TODO: something with camera basis
    /*SetForwardVector(forward);
    SetRightVector(right);
    SetUpVector(up);*/
    m_bIsDirty = false;
}

void Camera::UpdatePerspectiveProjection()
{
    m_perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(m_fovYRadians, m_aspectRatio, m_nearZ, m_farZ);
}

void Camera::UpdateOrthographicProjection()
{
    m_orthographicProjectionMatrix = XMMatrixOrthographicLH(GetNearWindowWidth(), GetNearWindowHeight(), m_nearZ, m_farZ);
}