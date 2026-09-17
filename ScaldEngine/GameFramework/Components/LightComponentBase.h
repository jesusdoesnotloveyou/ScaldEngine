#pragma once

#include "SceneComponent.h"
#include "Graphics/LightHelper.h"
#include "Graphics/Mesh.h"

namespace Scald
{
    class LightComponentBase : public SceneComponent
    {
        using Super = SceneComponent;
    public:
        LightComponentBase(std::shared_ptr<Actor> owner);
        virtual ~LightComponentBase();

        // to update dynamic properties of light objects (position, direction, intensity etc.)
        virtual void Tick(float deltaTime) override;
        void DrawLightVolume(ID3D11DeviceContext* pDeviceContext);
        
        // 4th parameter is intensity
        void SetAmbientColor(float x, float y, float z, float w);
        XMFLOAT4 GetAmbientColor() const;                        
        void SetDiffuseColor(float x, float y, float z, float w);
        XMFLOAT4 GetDiffuseColor() const;
        void SetSpecularColor(float x, float y, float z, float w);
        XMFLOAT4 GetSpecularColor() const;
    
        // dir and spot lights only
        void SetLookAt(XMFLOAT3 at);
        void SetDirection(float x, float y, float z);
        XMFLOAT3 GetDirection() const { m_lightParams.direction; }
        
        // point and spot lights only
        void SetRange(const float radius = 1.0f);
        float GetRange() const { return m_lightParams.range; }
        void SetAttenuation(float x, float y, float z);
        XMFLOAT3 GetAttenuation() const { m_lightParams.attenuation; }
    
    public:
        FORCEINLINE ELightType GetLightType() const { return m_lightType; }

        const XMMATRIX& GetViewMatrix() const;
        const XMMATRIX& GetPerspectiveProjectionMatrix() const;
        const XMMATRIX& GetOrthographicProjectionMatrix() const;

    private:
        void UpdateLightParams();

        void UpdateView();
        void UpdatePerspectiveProjection(float nearZ, float farZ);
        void UpdateOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ);

    protected:
        // Light properties
        LIGHT_DESC m_lightParams{};
        ELightType m_lightType = ELightType::None;

        XMMATRIX m_view;
        XMMATRIX m_perspectiveProjection;
        XMMATRIX m_orthographicProjection;
        
        XMFLOAT3 m_lookAt;

        Mesh m_lightVolume;
    };
} // namespace Scald