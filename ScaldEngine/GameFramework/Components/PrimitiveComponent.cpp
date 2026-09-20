#include "stdafx.h"
#include "PrimitiveComponent.h"
#include "GameFramework/Objects/Actor.h"
#include "GameFramework/World.h"
#include "Graphics/Scene/PrimitiveSceneProxy.h"
#include "Graphics/Scene/Scene.h"

using namespace Scald;

PrimitiveComponent::PrimitiveComponent()
{

}

PrimitiveComponent::PrimitiveComponent(std::shared_ptr<Actor> owner) 
    : Super(owner)
{
}

PrimitiveComponent::~PrimitiveComponent() noexcept
{

}

void PrimitiveComponent::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

PrimitiveSceneProxy* PrimitiveComponent::GetSceneProxy() const
{
     return m_sceneProxy.get();
}

void PrimitiveComponent::OnRegister()
{
    m_sceneProxy = CreateSceneProxy();
    if (m_sceneProxy && GetWorld())
    {
        // TODO: probably remove scene from here
        GetWorld()->GetScene()->AddPrimitive(m_sceneProxy.get());
    }
}

void PrimitiveComponent::OnUnregister()
{

}