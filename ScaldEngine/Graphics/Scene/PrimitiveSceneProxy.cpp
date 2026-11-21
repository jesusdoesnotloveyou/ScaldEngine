#include "PrimitiveSceneProxy.h"
#include "Graphics/Model.h"
#include "Graphics/Texture.h"

using namespace Scald;
using namespace DirectX;

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

const XMMATRIX PrimitiveSceneProxy::GetWorld() const
{
    return XMLoadFloat4x4(&m_world);
}

void PrimitiveSceneProxy::SetModel(const Model* model)
{
    m_model = model;
}

const Model* PrimitiveSceneProxy::GetModel() const
{
    return m_model;
}

void PrimitiveSceneProxy::SetTexture(const Texture* texture)
{
    m_texture = texture;
}

const Texture* PrimitiveSceneProxy::GetTexture() const
{
    return m_texture;
}