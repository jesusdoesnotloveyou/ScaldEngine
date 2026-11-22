#pragma once

#include "GameFramework/Components/SceneComponent.h"
#include "Graphics/LightHelper.h"

namespace Scald
{
    class LightSceneProxy;

    // Light component is a scene component since it has transform in the World
    class LightComponentBase : public SceneComponent
    {
        using Super = SceneComponent;
    public:
        LightComponentBase();
        LightComponentBase(std::shared_ptr<Actor> owner);
        virtual ~LightComponentBase();

        //~ Begin of ScaldActorComponent interface
        virtual void OnRegister() override;
        virtual void OnUnregister() override;
        //~ End of ScaldActorComponent interface
        virtual void Tick(float deltaTime) override;
        
        // White light, intensity (w) = 100%
        XMFLOAT4 LightColor     = {1.0f, 1.0f, 1.0f, 1.0f};
        float m_indirectScale   = 0.0f;
        float m_volumetricScale = 0.0f;

        uint8_t bAffectsWorld : 1u;
        uint8_t bCastShadows  : 1u;

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
    };
} // namespace Scald