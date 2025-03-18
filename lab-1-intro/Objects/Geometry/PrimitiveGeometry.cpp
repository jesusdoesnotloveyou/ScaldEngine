#include "../../ScaldException.h"
#include "PrimitiveGeometry.h"
#include <sstream>

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent{};
    pCollisionComponent = new CollisionComponent{};
    pMovementComponent = new MovementComponent{};
}

PrimitiveGeometry::PrimitiveGeometry(const STransform& transform) : PrimitiveGeometry()
{
    ObjectTransform = transform;
    constantBuffer.SetTransform(ObjectTransform);
    pCollisionComponent->SetCenter(transform.Translation);
    pCollisionComponent->SetExtends(transform.Scale);
}

PrimitiveGeometry::PrimitiveGeometry(const std::vector<Vertex>& v, const std::vector<DWORD>& i)
    : PrimitiveGeometry()
{
    vertices = v;
    indeces = i;
}

PrimitiveGeometry::~PrimitiveGeometry()
{
    if (pRenderComponent) delete pRenderComponent;
    if (pCollisionComponent) delete pCollisionComponent;
    if (pMovementComponent) delete pMovementComponent;
}

void PrimitiveGeometry::Update(float DeltaTime)
{   
    //constantBuffer.SetTransform(ObjectTransform);
    pCollisionComponent->UpdateOwnerTransform(ObjectTransform);
}

void PrimitiveGeometry::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    //pRenderComponent->Initialize(pDevice);
    pCollisionComponent->Initialize(ObjectTransform);

    ThrowIfFailed(vertexBuffer.Init(pDevice, vertices.data(), (UINT)vertices.size()));
    ThrowIfFailed(indexBuffer.Init(pDevice, indeces.data(), (UINT)indeces.size()));
    ThrowIfFailed(constantBuffer.Init(pDevice, pDeviceContext));
}

VertexBuffer<Vertex>& PrimitiveGeometry::GetVertexBuffer()
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

void PrimitiveGeometry::Reset(const XMFLOAT3& newSpeed, const XMFLOAT3& newTranslation)
{
    GetMovementComponent()->SetVelocity(newSpeed);
    ObjectTransform.Translation = newTranslation;
}