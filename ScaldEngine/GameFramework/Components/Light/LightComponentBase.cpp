#include "stdafx.h"
#include "LightComponentBase.h"

#include "GameFramework/Objects/Actor.h"

using namespace Scald;

// Fpr unique_ptr
LightComponentBase::LightComponentBase()
{

}

LightComponentBase::LightComponentBase(std::shared_ptr<AActor> owner)
    : Super(owner)
    /*, m_lookAt(0.0f, 0.0f, 0.0f)
    , m_view(XMMatrixIdentity())
    , m_perspectiveProjection(XMMatrixIdentity())
    , m_orthographicProjection(XMMatrixIdentity())*/
{
    //m_collisionComponent->DisableCollision();
}

LightComponentBase::~LightComponentBase()
{
}

void LightComponentBase::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    //UpdateLightParams();

    /*UpdateView();
    UpdatePerspectiveProjection(1.0f, 100.0f);
    UpdateOrthographicProjection(100.0f, 100.0f, 0.1f, 500.0f);*/
}

void LightComponentBase::OnRegister() 
{
}

void LightComponentBase::OnUnregister()
{
}

/*void LightComponentBase::SetAmbientColor(float x, float y, float z, float w)
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
}*/

//void LightComponentBase::SetDirection(float x, float y, float z)
//{
//    XMStoreFloat3(&m_lightParams.direction, XMVector3Normalize(XMVectorSet(x, y, z, 0.0f)));
//}
//