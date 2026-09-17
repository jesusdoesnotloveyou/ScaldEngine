#include "PrimitiveSceneProxy.h"

using namespace Scald;

PrimitiveSceneProxy::PrimitiveSceneProxy()
{

}

PrimitiveSceneProxy::~PrimitiveSceneProxy()
{

}

void PrimitiveSceneProxy::Draw() const
{
    m_model.Draw();
}

void PrimitiveSceneProxy::UpdateObjectCBs(float deltaTime)
{
    // TODO: make void const and move const buffer logic to UpdateCB method
    ConstantBufferPerObject bufferVS = {};

    /*const XMMATRIX world = GetTransform()->mWorldMatrix;
    auto det = XMMatrixDeterminant(world);

    const XMMATRIX invTransWorld = XMMatrixInverse(&det, XMMatrixTranspose(world));

    bufferVS.gWorld = XMMatrixTranspose(world);
    bufferVS.gInvTransWorld = XMMatrixTranspose(invTransWorld);*/

    m_model.GetConstantBufferVS().SetAndApplyData(bufferVS);
}