#include "../../../ScaldException.h"
#include "PrimitiveGeometry.h"
#include <sstream>

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent{};
    pCollisionComponent = new CollisionComponent{};
    pMovementComponent = new MovementComponent{};
}

PrimitiveGeometry::PrimitiveGeometry(const std::tuple<std::vector<VertexTex>, std::vector<DWORD>>& vi)
{}

PrimitiveGeometry::~PrimitiveGeometry()
{
    if (pRenderComponent) delete pRenderComponent;
    if (pCollisionComponent) delete pCollisionComponent;
    if (pMovementComponent) delete pMovementComponent;
}

void PrimitiveGeometry::Update(const ScaldTimer& st)
{
    UpdateRotation(st);
    UpdateOrbitRotation(st);

    UpdateWorldMatrix();
}

void PrimitiveGeometry::UpdateOrbitRotation(const ScaldTimer& st)
{
    ObjectTransform.orbitRot += XMConvertToRadians(ObjectTransform.orbitAngle) * st.DeltaTime();
    if (ObjectTransform.orbitRot >= 6.28f)
        ObjectTransform.orbitRot = 0.0f;
}

void PrimitiveGeometry::UpdateRotation(const ScaldTimer& st)
{
    ObjectTransform.rot += XMConvertToRadians(ObjectTransform.rotAngle) * st.DeltaTime();
    if (ObjectTransform.rot >= 6.28f)
        ObjectTransform.rot = 0.0f;
}

void PrimitiveGeometry::UpdateLocalMatrix()
{
    // SRT - default order of matrix multiplication
    // (S)TR - orbit effect for Solar system could be used
    ObjectTransform.mLocalMatrix =
        XMMatrixScaling(ObjectTransform.Scale.x, ObjectTransform.Scale.y, ObjectTransform.Scale.z) *
        XMMatrixRotationY(ObjectTransform.rot) *
        XMMatrixTranslation(ObjectTransform.Translation.x, ObjectTransform.Translation.y, ObjectTransform.Translation.z) *
        XMMatrixRotationY(ObjectTransform.orbitRot);
}

void PrimitiveGeometry::UpdateWorldMatrix()
{
    UpdateLocalMatrix();

    if (ObjectTransform.ParentTransform)
    {
        XMVECTOR outScale;
        XMVECTOR outRot;
        XMVECTOR outTrans;
        XMMatrixDecompose(&outScale, &outRot, &outTrans, ObjectTransform.ParentTransform->mWorldMatrix);
        ObjectTransform.mWorldMatrix = ObjectTransform.mLocalMatrix * XMMatrixTranslationFromVector(outTrans);
    }
    else
    {
        ObjectTransform.mWorldMatrix = ObjectTransform.mLocalMatrix;
    }
}

void PrimitiveGeometry::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* pDeviceContext)
{
    ThrowIfFailed(vertexBuffer.Init(mDevice, vertices.data(), (UINT)vertices.size()));
    ThrowIfFailed(indexBuffer.Init(mDevice, indeces.data(), (UINT)indeces.size()));
    ThrowIfFailed(constantBuffer.Init(mDevice, pDeviceContext));
    constantBuffer.SetTransform(&ObjectTransform);

    ObjectTransform.Translation.x = ObjectTransform.orbitRadius;
}

void PrimitiveGeometry::UpdateObjectCBs(const ScaldTimer& st)
{
    constantBuffer.SetTransform(&ObjectTransform);
}

VertexBuffer<VertexTex>& PrimitiveGeometry::GetVertexBuffer()
{
    return vertexBuffer;
}

IndexBuffer& PrimitiveGeometry::GetIndexBuffer()
{
    return indexBuffer;
}

ConstantBuffer<ConstBuffer>& PrimitiveGeometry::GetConstantBuffer()
{
    return constantBuffer;
}

RenderComponent* PrimitiveGeometry::GetRenderComponent() const
{
    return pRenderComponent;
}

CollisionComponent* PrimitiveGeometry::GetCollisionComponent() const
{
    return pCollisionComponent;
}

MovementComponent* PrimitiveGeometry::GetMovementComponent() const
{
    return pMovementComponent;
}