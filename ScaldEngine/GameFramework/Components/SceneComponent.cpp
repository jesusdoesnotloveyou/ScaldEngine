#include "stdafx.h"
#include "SceneComponent.h"
#include "Transform.h"

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
    //m_transform.Tick(deltaTime);
}

const Transform& SceneComponent::GetComponentTransform() const
{
    // dead ref, temparary
    return Transform{};
}

XMVECTOR SceneComponent::GetPosition() const
{
    return m_transform.GetPositionVector();
}

XMFLOAT3 SceneComponent::GetPositionFloat() const
{
    return m_transform.GetPositionFloat3();
}

XMVECTOR SceneComponent::GetRotation() const
{
    return m_transform.GetRotationVector();
}

XMVECTOR SceneComponent::GetOrientation() const
{
    return m_transform.GetOrientation();
}

XMVECTOR SceneComponent::GetScale() const
{
    return m_transform.GetScaleVector();
}

void SceneComponent::SetPosition(const XMVECTOR& pos)
{
    m_transform.SetPosition(pos);
    UpdateAttachChildren();
}

void SceneComponent::SetPosition(float x, float y, float z)
{
    m_transform.SetPosition(x, y, z);
    UpdateAttachChildren();
}

void SceneComponent::AdjustPosition(const XMVECTOR& pos)
{
    m_transform.AdjustPosition(pos);
    UpdateAttachChildren();
}

void SceneComponent::AdjustPosition(float x, float y, float z)
{
    m_transform.AdjustPosition(x, y, z);
    UpdateAttachChildren();
}

void SceneComponent::UpdateAttachChildren()
{
    if (m_attachChildren.empty()) return;
    for (auto child : m_attachChildren)
    {
        child->GetTransform().UpdateWorldMatrix();
    }
}

void SceneComponent::SetOrientation(const XMVECTOR& newRotation)
{
    m_transform.SetOrientation(newRotation);
    if (m_attachChildren.empty()) return;
    for (auto child : m_attachChildren)
    {
        child->GetTransform().UpdateWorldMatrix();
    }
}

void SceneComponent::SetRotation(const XMVECTOR& rot)
{
    m_transform.SetRotation(rot);
}

void SceneComponent::SetRotation(float x, float y, float z)
{
    m_transform.SetRotation(x, y, z);
}

void SceneComponent::AdjustRotation(const XMVECTOR& rot)
{
    m_transform.AdjustRotation(rot);
}

void SceneComponent::AdjustRotation(float x, float y, float z)
{
    m_transform.AdjustRotation(x, y, z);
}

void SceneComponent::SetScale(const XMVECTOR& scale)
{
    m_transform.SetScale(scale);
}

void SceneComponent::SetScale(float x, float y, float z)
{
    m_transform.SetScale(x, y, z);
}

void SceneComponent::AdjustScale(const XMVECTOR& scale)
{
    m_transform.AdjustScale(scale);
}

void SceneComponent::AdjustScale(float x, float y, float z)
{
    m_transform.AdjustScale(x, y, z);
}

XMVECTOR SceneComponent::GetForwardVector() const
{
    return m_transform.GetForwardVector();
}

XMVECTOR SceneComponent::GetRightVector() const
{
    return m_transform.GetRightVector();
}

XMVECTOR SceneComponent::GetUpVector() const
{
    return m_transform.GetUpVector();
}

void SceneComponent::SetForwardVector(const XMVECTOR& relativeForwardVector)
{
    m_transform.SetForwardVector(relativeForwardVector);
}

void SceneComponent::SetRightVector(const XMVECTOR& relativeRightVector)
{
    m_transform.SetRightVector(relativeRightVector);
}

void SceneComponent::SetUpVector(const XMVECTOR& relativeUpVector)
{
    m_transform.SetUpVector(relativeUpVector);
}

void SceneComponent::AttachToParent(SceneComponent* parent)
{
    if (!parent) return;

    parent->m_attachChildren.push_back(this);

    m_attachParent = parent;
    m_transform.SetParentTransform(parent->GetTransform());
}

Transform& SceneComponent::GetTransform()
{
    return m_transform;
}