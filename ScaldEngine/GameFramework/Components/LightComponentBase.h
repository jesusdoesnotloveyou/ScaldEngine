#pragma once

#include "SceneComponent.h"
#include "Graphics/LightHelper.h"

namespace Scald
{
    class LightSceneProxy;

    class LightComponentBase : public SceneComponent
    {
        using Super = SceneComponent;
    public:
        LightComponentBase(std::shared_ptr<Actor> owner);
        virtual ~LightComponentBase();

        // to update dynamic properties of light objects (position, direction, intensity etc.)
        virtual void Tick(float deltaTime) override;
        virtual void OnRegister() override;
        virtual void OnUnregister() override;
        
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
        //XMFLOAT3 GetDirection() const { m_lightParams.direction; }
        
        // point and spot lights only
        void SetRange(const float radius = 1.0f);
        //float GetRange() const { return m_lightParams.range; }
        void SetAttenuation(float x, float y, float z);
        //XMFLOAT3 GetAttenuation() const { m_lightParams.attenuation; }
    
    public:
        const XMMATRIX& GetViewMatrix() const;
        const XMMATRIX& GetPerspectiveProjectionMatrix() const;
        const XMMATRIX& GetOrthographicProjectionMatrix() const;

    private:
        void UpdateLightParams();

        // Mandatory for directional light. Probably for the spot and point lights too when I will implement shadows for these types.
        void UpdateView();
        void UpdatePerspectiveProjection(float nearZ, float farZ);
        void UpdateOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ);
        //

    protected:
        // LightComponents subclasses (direct?, point, spot) provide proxies
        virtual std::unique_ptr<LightSceneProxy> CreateSceneProxy() /* = 0 */;
        std::unique_ptr<LightSceneProxy> m_lightProxy;

        // TODO: probably should be moved to LightSceneProxy
        // Light properties
        //LIGHT_DESC m_lightParams{};
    
        // TODO: Consider removing matrices from class and creating them in runtime by demand
        XMMATRIX m_view;
        XMMATRIX m_perspectiveProjection;
        XMMATRIX m_orthographicProjection;
        
        XMFLOAT3 m_lookAt;
    };
} // namespace Scald