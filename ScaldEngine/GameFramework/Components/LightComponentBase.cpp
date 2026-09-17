#include "stdafx.h"
#include "LightComponentBase.h"
#include "GameFramework/Objects/Actor.h"
#include "Utils/Shapes.h"

using namespace Scald;

LightComponentBase::LightComponentBase(std::shared_ptr<Actor> owner)
    : Super(owner)
    , m_lookAt(0.0f, 0.0f, 0.0f)
    , m_view(XMMatrixIdentity())
    , m_perspectiveProjection(XMMatrixIdentity())
    , m_orthographicProjection(XMMatrixIdentity())
{
    //m_collisionComponent->DisableCollision();
}

LightComponentBase::~LightComponentBase()
{
}

void LightComponentBase::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    UpdateLightParams();
    //if (LightType == ELightType::Directional)
    //{
    //}

    if (m_lightType == ELightType::Point || m_lightType == ELightType::Spot)
    {
        std::vector<VertexPositionNormalUV> volumeVertices;
        std::vector<DWORD> volumeIndices;
        Shapes::GetSphereShape(volumeVertices, volumeIndices, 1.0f /*hard - coded value just for now*/, 8, 16);

    }

    UpdateView();
    UpdatePerspectiveProjection(1.0f, 100.0f);
    UpdateOrthographicProjection(100.0f, 100.0f, 0.1f, 500.0f);
}

void LightComponentBase::UpdateLightParams()
{
    SetLookAt(m_lightParams.direction);
}

void LightComponentBase::UpdateView()
{
    XMFLOAT3 up = {0.0f, 1.0f, 0.0f};
    XMVECTOR lookAtVector, upVector;
    XMVECTOR pos = GetPosition();

    // Load the XMFLOAT3 into XMVECTOR.
    lookAtVector = XMLoadFloat3(&m_lookAt);
    upVector = XMLoadFloat3(&up);

    // Create the view matrix from the three vectors.
    m_view = XMMatrixLookAtLH(pos, lookAtVector, upVector);
}

void LightComponentBase::UpdatePerspectiveProjection(float nearZ, float farZ) {}

void LightComponentBase::UpdateOrthographicProjection(float viewWidth, float viewHeight, float nearZ, float farZ)
{
    m_orthographicProjection = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
}

void LightComponentBase::DrawLightVolume(ID3D11DeviceContext* pDeviceContext)
{
    // might be good idea to encapsulate light mesh draw here whatever kind of light it is
    // light volume could be sphere, cone or quad
    auto& lightVB = m_lightVolume.GetVertexBuffer();
    auto& lightIB = m_lightVolume.GetIndexBuffer();

    pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pDeviceContext->IASetVertexBuffers(0u, 1u, lightVB.GetAddressOf(), lightVB.GetStridePtr(), lightVB.GetOffsetPtr());
    pDeviceContext->IASetIndexBuffer(lightIB.Get(), DXGI_FORMAT_R32_UINT, 0u);
    /*if (lightIB is empty)
    {
        pDeviceContext->Draw(lightVB.GetBufferSize(), 0u);
    }
    else*/
    pDeviceContext->DrawIndexed(lightIB.GetBufferSize(), 0u, 0);
}

void LightComponentBase::SetAmbientColor(float x, float y, float z, float w)
{
    m_lightParams.ambient = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 LightComponentBase::GetAmbientColor() const
{
    return m_lightParams.ambient;
}

void LightComponentBase::SetDiffuseColor(float x, float y, float z, float w)
{
    m_lightParams.diffuse = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 LightComponentBase::GetDiffuseColor() const
{
    return m_lightParams.diffuse;
}

void LightComponentBase::SetSpecularColor(float x, float y, float z, float w)
{
    m_lightParams.specular = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 LightComponentBase::GetSpecularColor() const
{
    return m_lightParams.specular;
}

void LightComponentBase::SetLookAt(XMFLOAT3 at)
{
    m_lookAt = at;
}

void LightComponentBase::SetDirection(float x, float y, float z)
{
    XMStoreFloat3(&m_lightParams.direction, XMVector3Normalize(XMVectorSet(x, y, z, 0.0f)));
}

void LightComponentBase::SetRange(const float radius)
{
    m_lightParams.range = radius;
}

void LightComponentBase::SetAttenuation(float x, float y, float z)
{
    m_lightParams.attenuation = XMFLOAT3(x, y, z);
}

const XMMATRIX& LightComponentBase::GetViewMatrix() const
{
    return m_view;
}

const XMMATRIX& LightComponentBase::GetPerspectiveProjectionMatrix() const
{
    return m_perspectiveProjection;
}

const XMMATRIX& LightComponentBase::GetOrthographicProjectionMatrix() const
{
    return m_orthographicProjection;
}