#pragma once

#include "LightComponent.h"
#include "Utils/Shapes.h"

namespace Scald
{
class LocalLightComponent : public LightComponent
{
    using Super = LightComponent;

public:
    virtual ~LocalLightComponent() override = default;

    // point and spot lights only
    void SetRange(const float radius = 1.0f);
    // float GetRange() const { return m_lightParams.range; }
    void SetAttenuation(float x, float y, float z);
    // XMFLOAT3 GetAttenuation() const { m_lightParams.attenuation; }
};

class PointLightComponent : public LocalLightComponent
{
    using Super = LocalLightComponent;

public:
    virtual ~PointLightComponent() override = default;
};

class SpotLightComponent : public LocalLightComponent
{
    using Super = LocalLightComponent;

public:
    virtual ~SpotLightComponent() override = default;
};

}  // namespace Scald