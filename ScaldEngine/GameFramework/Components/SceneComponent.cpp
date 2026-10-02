#include "stdafx.h"
#include "SceneComponent.h"

using namespace Scald;

SceneComponent::SceneComponent(std::shared_ptr<Actor> owner)
    : Super(owner)
{

}

SceneComponent::~SceneComponent()
{
}

void SceneComponent::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    //m_componentToWorld.Tick(deltaTime);
}

void SceneComponent::OnRegister() {}

void SceneComponent::OnUnregister() {}

const Transform& SceneComponent::GetComponentTransform() const
{
    return m_componentToWorld;
}

XMVECTOR SceneComponent::GetPosition() const
{
    return m_componentToWorld.GetPositionVector();
}

XMFLOAT3 SceneComponent::GetPositionFloat() const
{
    return m_componentToWorld.GetPositionFloat3();
}

XMVECTOR SceneComponent::GetRotation() const
{
    return m_componentToWorld.GetRotationVector();
}

XMVECTOR SceneComponent::GetOrientation() const
{
    return m_componentToWorld.GetOrientation();
}

XMVECTOR SceneComponent::GetScale() const
{
    return m_componentToWorld.GetScaleVector();
}

void SceneComponent::SetPosition(const XMVECTOR& pos)
{
    m_componentToWorld.SetPosition(pos);
    UpdateAttachChildren();
}

void SceneComponent::SetPosition(float x, float y, float z)
{
    m_componentToWorld.SetPosition(x, y, z);
    UpdateAttachChildren();
}

void SceneComponent::AdjustPosition(const XMVECTOR& pos)
{
    m_componentToWorld.AdjustPosition(pos);
    UpdateAttachChildren();
}

void SceneComponent::AdjustPosition(float x, float y, float z)
{
    m_componentToWorld.AdjustPosition(x, y, z);
    UpdateAttachChildren();
}

void SceneComponent::SetOrientation(const XMVECTOR& newRotation)
{
    m_componentToWorld.SetOrientation(newRotation);
    m_bIsDirty = true;
    // TODO: not here
    //UpdateAttachChildren();
}

void SceneComponent::SetRotation(const XMVECTOR& rot)
{
    m_componentToWorld.SetRotation(rot);
    m_bIsDirty = true;
}

void SceneComponent::SetRotation(float x, float y, float z)
{
    m_componentToWorld.SetRotation(x, y, z);
    m_bIsDirty = true;
}

void SceneComponent::AdjustRotation(const XMVECTOR& rot)
{
    m_componentToWorld.AdjustRotation(rot);
    m_bIsDirty = true;
}

void SceneComponent::AdjustRotation(float x, float y, float z)
{
    m_componentToWorld.AdjustRotation(x, y, z);
    m_bIsDirty = true;
}

void SceneComponent::SetScale(const XMVECTOR& scale)
{
    m_componentToWorld.SetScale(scale);
    m_bIsDirty = true;
}

void SceneComponent::SetScale(float x, float y, float z)
{
    m_componentToWorld.SetScale(x, y, z);
    m_bIsDirty = true;
}

void SceneComponent::AdjustScale(const XMVECTOR& scale)
{
    m_componentToWorld.AdjustScale(scale);
    m_bIsDirty = true;
}

void SceneComponent::AdjustScale(float x, float y, float z)
{
    m_componentToWorld.AdjustScale(x, y, z);
    m_bIsDirty = true;
}

XMVECTOR SceneComponent::GetForwardVector() const
{
    return m_componentToWorld.GetForwardVector();
}

XMVECTOR SceneComponent::GetRightVector() const
{
    return m_componentToWorld.GetRightVector();
}

XMVECTOR SceneComponent::GetUpVector() const
{
    return m_componentToWorld.GetUpVector();
}

void SceneComponent::SetupAttachment(SceneComponent* inParent, const char* inSocketName)
{
    AttachToParent(inParent);
    // m_bIsAttached = true;
}

void SceneComponent::DetachFromComponent()
{
    // GetTransform().SetParentTransform();
    // m_bIsAttached = false;

    // TODO: remove parent
    // TODO: detach from parent transform
}

void SceneComponent::AttachToParent(SceneComponent* parent)
{
    if (!parent) return;

    parent->m_attachChildren.push_back(this);

    m_attachParent = parent;
    // TODO: something with parent transform
    //m_componentToWorld.SetParentTransform(parent->GetTransform());
}

void SceneComponent::UpdateAttachChildren()
{
    if (m_attachChildren.empty()) return;
    for (auto child : m_attachChildren)
    {
        child->UpdateComponentToWorld();
    }
}

void SceneComponent::UpdateComponentToWorld()
{
    // SRT - default order of matrix multiplication
    // (S)TR - orbit effect for Solar system could be used
    // mWorldMatrix = XMMatrixScalingFromVector(GetScaleVector()) * XMMatrixRotationQuaternion(mQuaternionRotation) * XMMatrixTranslationFromVector(GetPositionVector());
    if (m_attachParent)
    {
        m_attachParent->UpdateComponentToWorld();
    }

    /* if (m_parentTransform)
     {
         mWorldMatrix *= XMMatrixRotationQuaternion(m_parentTransform->GetOrientation()) * XMMatrixTranslationFromVector(m_parentTransform->GetPositionVector());
     }*/
}