#include "stdafx.h"
#include "GameFramework/Components/StaticMeshComponent.h"
#include "Graphics/Scene/StaticMeshSceneProxy.h"

using namespace Scald;

std::unique_ptr<PrimitiveSceneProxy> StaticMeshComponent::CreateSceneProxy()
{
    return std::make_unique<StaticMeshSceneProxy>();
}