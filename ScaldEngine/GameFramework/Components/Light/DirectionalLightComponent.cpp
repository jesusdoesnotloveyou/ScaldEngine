#include "DirectionalLightComponent.h"

using namespace Scald;

const XMMATRIX DirectionalLightComponent::GetViewMatrix() const
{
    return XMLoadFloat4x4(&m_view);
}

const XMMATRIX DirectionalLightComponent::GetPerspectiveProjectionMatrix() const
{
    return XMLoadFloat4x4(&m_perspectiveProjection);
}

const XMMATRIX DirectionalLightComponent::GetOrthographicProjectionMatrix() const
{
    return XMLoadFloat4x4(&m_orthographicProjection);
}

void DirectionalLightComponent::SetLookAt(XMFLOAT3 at)
{
    m_lookAt = at;
}

void DirectionalLightComponent::UpdateLightParams()
{
    //SetLookAt(m_lightParams.direction);
}

void DirectionalLightComponent::UpdateView()
{
    XMFLOAT3 up = {0.0f, 1.0f, 0.0f};
    XMVECTOR lookAtVector, upVector;
    XMVECTOR pos = GetPosition();

    // Load the XMFLOAT3 into XMVECTOR.
    lookAtVector = XMLoadFloat3(&m_lookAt);
    upVector = XMLoadFloat3(&up);

    // Create the view matrix from the three vectors.
    XMStoreFloat4x4(&m_view, XMMatrixLookAtLH(pos, lookAtVector, upVector));
}

void DirectionalLightComponent::UpdatePerspectiveProjection(float nearZ, float farZ) {}

void DirectionalLightComponent::UpdateOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
{
    XMStoreFloat4x4(&m_orthographicProjection, XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ));
}