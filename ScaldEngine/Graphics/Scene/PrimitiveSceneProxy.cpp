#include "PrimitiveSceneProxy.h"

using namespace Scald;

PrimitiveSceneProxy::PrimitiveSceneProxy()
{

}

PrimitiveSceneProxy::~PrimitiveSceneProxy()
{

}

void PrimitiveSceneProxy::SetWorld(XMMATRIX world)
{
    XMStoreFloat4x4(&m_world, world);
}

void PrimitiveSceneProxy::SetModel(const Model* model)
{
    m_model = model;
}

const XMMATRIX PrimitiveSceneProxy::GetWorld() const
{
    return XMLoadFloat4x4(&m_world);
}

const Model* PrimitiveSceneProxy::GetModel() const
{
    return m_model;
}