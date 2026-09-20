#include "Scene.h"
#include "PrimitiveSceneProxy.h"
#include "LightSceneProxy.h"

#include <algorithm>
#include <utility>

using namespace Scald;

void Scene::AddPrimitive(PrimitiveSceneProxy* proxy)
{
    // mutex lock
    m_primitives.push_back(proxy);
}

void Scene::RemovePrimitive(PrimitiveSceneProxy* proxy)
{
    // mutex lock
    
    // assert
    if (!proxy) return;
    
    if (m_primitives.back() == proxy)
    {
        m_primitives.pop_back();
        return;
    }

    auto it = std::find(m_primitives.begin(), m_primitives.end(), proxy);
    if (it == m_primitives.end())
    {
        // assert
        return;
    }

    std::swap(*it, m_primitives.back());
    m_primitives.pop_back();
    return;
}

void Scene::UpdatePrimitiveTransform(PrimitiveSceneProxy* proxy, const Transform& componentTransform)
{
    proxy->SetWorld(
        XMMatrixScalingFromVector(componentTransform.GetScaleVector()) * 
        XMMatrixRotationRollPitchYawFromVector(componentTransform.GetOrientation()) * 
        XMMatrixTranslationFromVector(componentTransform.GetPositionVector()));
}

void Scene::AddLight(LightSceneProxy* proxy)
{
    // mutex lock
    m_lights.push_back(proxy);
}

void Scene::RemoveLight(LightSceneProxy* proxy)
{
    // mutex lock

    // assert
    if (!proxy) return;

    if (m_lights.back() == proxy)
    {
        m_lights.pop_back();
        return;
    }

    auto it = std::find(m_lights.begin(), m_lights.end(), proxy);
    if (it == m_lights.end())
    {
        // assert
        return;
    }

    std::swap(*it, m_lights.back());
    m_lights.pop_back();
    return;
}