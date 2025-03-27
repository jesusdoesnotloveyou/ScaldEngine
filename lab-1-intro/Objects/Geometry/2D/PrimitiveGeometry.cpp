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
    ObjectTransform.rotationAngle += XMConvertToRadians(ObjectTransform.rotationSpeed) * st.DeltaTime();
    ObjectTransform.Rotation.y = ObjectTransform.rotationAngle;

    UpdateWorldMatrix();
}

void PrimitiveGeometry::UpdateWorldMatrix()
{
    // SRT - default order of matrix multiplication
    // (S)TR - orbit effect for Solar system could be used
    ObjectTransform.mLocalMatrix = XMMatrixScaling(ObjectTransform.Scale.x, ObjectTransform.Scale.y, ObjectTransform.Scale.z) *
        XMMatrixTranslation(ObjectTransform.Translation.x, ObjectTransform.Translation.y, ObjectTransform.Translation.z) *
        XMMatrixRotationRollPitchYaw(ObjectTransform.Rotation.x, ObjectTransform.Rotation.y, ObjectTransform.Rotation.z);

    if (ObjectTransform.ParentTransform)
    {
        ObjectTransform.mWorldMatrix = ObjectTransform.mLocalMatrix * ObjectTransform.ParentTransform->mWorldMatrix;
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