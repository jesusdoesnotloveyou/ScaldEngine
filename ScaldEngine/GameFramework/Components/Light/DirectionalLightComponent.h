#pragma once

#include "LightComponent.h"
#include <DirectXMath.h>

namespace Scald
{
class DirectionalLightComponent : public LightComponent
{
    using Super = LightComponent;
public:
    DirectionalLightComponent() = default;
    virtual ~DirectionalLightComponent() override = default;

    const XMMATRIX GetViewMatrix() const;
    const XMMATRIX GetPerspectiveProjectionMatrix() const;
    const XMMATRIX GetOrthographicProjectionMatrix() const;

    // dir and spot lights only
    void SetLookAt(XMFLOAT3 at);
private:
    void UpdateLightParams();

    // Mandatory for directional light. Probably for the spot and point lights too when I will implement shadows for these types.
    void UpdateView();
    void UpdatePerspectiveProjection(float nearZ, float farZ);
    void UpdateOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ);
    //

protected:
    // TODO: Consider removing matrices from class and creating them in runtime by demand
    XMFLOAT4X4 m_view;
    XMFLOAT4X4 m_perspectiveProjection;
    XMFLOAT4X4 m_orthographicProjection;
    
    XMFLOAT3 m_lookAt;
};
}  // namespace Scald