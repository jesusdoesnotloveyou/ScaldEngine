#include "stdafx.h"
#include "GameFramework/Components/StaticMeshComponent.h"
#include "Graphics/Scene/StaticMeshSceneProxy.h"

using namespace Scald;

void StaticMeshComponent::SetStaticMesh(const Model* staticMeshModel)
{
    m_sceneProxy->SetModel(staticMeshModel);
}

void StaticMeshComponent::SetTexture(const Texture* meshTexture)
{
    m_sceneProxy->SetTexture(meshTexture);
}

std::unique_ptr<PrimitiveSceneProxy> StaticMeshComponent::CreateSceneProxy()
{
    return std::make_unique<StaticMeshSceneProxy>();
}