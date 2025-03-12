#include "../../ScaldException.h"
#include "PrimitiveGeometry.h"

PrimitiveGeometry::PrimitiveGeometry()
{
    pRenderComponent = new RenderComponent{};
    pCollisionComponent = new CollisionComponent{};
    pMovementComponent = new MovementComponent{};
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
    if (bIsMovable)
    {
        if (ObjectTransform.Translation.y - ObjectTransform.Scale.y > 1.0f || ObjectTransform.Translation.y + ObjectTransform.Scale.y < -1.0f)
        {
            auto speed = GetMovementComponent()->GetVelocity();
            GetMovementComponent()->SetVelocity(speed.x, speed.y * (-1.0f), speed.z);
        }
        if (ObjectTransform.Translation.x - ObjectTransform.Scale.x > 1.0f || ObjectTransform.Translation.x + ObjectTransform.Scale.x < -1.0f)
        {
            auto speed = GetMovementComponent()->GetVelocity();
            GetMovementComponent()->SetVelocity(speed.x * (-1.0f), speed.y, speed.z);
        }
        ObjectTransform.Translation.x += GetMovementComponent()->GetVelocity().x;
        ObjectTransform.Translation.y += GetMovementComponent()->GetVelocity().y;
    }
    constantBuffer.SetTransform(ObjectTransform);
    pCollisionComponent->UpdateOwnerTransform(ObjectTransform);
}

void PrimitiveGeometry::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    pRenderComponent->Initialize(pDevice);
    pCollisionComponent->Initialize(ObjectTransform);
    //pInputComponent->Initialize();

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