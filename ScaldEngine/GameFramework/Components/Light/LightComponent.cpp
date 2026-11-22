#include "LightComponent.h"
#include "GameFramework/World.h"
#include "Graphics/Scene/Scene.h"
#include "Graphics/Scene/LightSceneProxy.h"

using namespace Scald;

std::unique_ptr<LightSceneProxy> LightComponent::CreateSceneProxy()
{
    return std::make_unique<LightSceneProxy>();
}

void LightComponent::OnRegister()
{
    m_lightProxy = CreateSceneProxy();
    // TODO: no world yet for light components
    //if (m_lightProxy && GetWorld())
    //{
    //    // TODO: probably remove scene from here
    //    GetWorld()->GetScene()->AddLight(m_lightProxy.get());
    //}
}

void LightComponent::OnUnregister()
{
    GetWorld()->GetScene()->RemoveLight(m_lightProxy.get());
}