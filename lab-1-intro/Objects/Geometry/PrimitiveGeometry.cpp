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
    /*if (bIsMovable)
    {
        std::ostringstream oss;
        if (ObjectTransform.Translation.y - ObjectTransform.Scale.y > 1.0f || ObjectTransform.Translation.y + ObjectTransform.Scale.y < -1.0f)
        {
            auto speed = GetMovementComponent()->GetVelocity();
            GetMovementComponent()->SetVelocity(speed.x, speed.y * (-1.0f), speed.z);
        }
        if (ObjectTransform.Translation.x - ObjectTransform.Scale.x > 1.0f)
        {
            Reset(pMovementComponent->GetInitialVelocity(), pMovementComponent->GetInitialTransition());
            leftPlayerScore += 1;
            oss << "Left Player: " << leftPlayerScore << " || " << "Right Player: " << rightPlayerScore << "\n";
            OutputDebugString(oss.str().c_str());
        }

        if (ObjectTransform.Translation.x + ObjectTransform.Scale.x < -1.0f)
        {
            Reset(pMovementComponent->GetInitialVelocity(), pMovementComponent->GetInitialTransition());
            rightPlayerScore += 1;
            oss << "Left Player: " << leftPlayerScore << " || " << "Right Player: " << rightPlayerScore << "\n";
            OutputDebugString(oss.str().c_str());
        }
        ObjectTransform.Translation.x += GetMovementComponent()->GetVelocity().x;
        ObjectTransform.Translation.y += GetMovementComponent()->GetVelocity().y;
    }
    */
    constantBuffer.SetTransform(ObjectTransform);
    pCollisionComponent->UpdateOwnerTransform(ObjectTransform);
}

void PrimitiveGeometry::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    pRenderComponent->Initialize(pDevice);
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