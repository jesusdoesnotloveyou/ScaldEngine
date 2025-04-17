#include "SceneComponent.h"
#include "TransformComponent.h"

SceneComponent::SceneComponent()
{
	mTransformComponent = new TransformComponent();
}

SceneComponent::~SceneComponent()
{
	if (mTransformComponent) delete mTransformComponent;
}

void SceneComponent::Update(const ScaldTimer& st)
{
	//ScaldComponent::Update(st);
	mTransformComponent->Update(st);
}

void SceneComponent::SetPosition(const XMVECTOR& pos)
{
	mTransformComponent->SetPosition(pos);
}

void SceneComponent::SetPosition(float x, float y, float z)
{
	mTransformComponent->SetPosition(x, y, z);
}

void SceneComponent::AdjustPosition(const XMVECTOR& pos)
{
	mTransformComponent->AdjustPosition(pos);
	if (mChildren.empty()) return;
	for (auto child : mChildren)
	{
		child->GetTransform()->UpdateWorldMatrix();
	}
}

void SceneComponent::AdjustPosition(float x, float y, float z)
{
	mTransformComponent->AdjustPosition(x, y, z);
}

void SceneComponent::SetOrientation(const XMVECTOR& newRotation)
{
	mTransformComponent->SetOrientation(newRotation);
	if (mChildren.empty()) return;
	for (auto child : mChildren)
	{
		child->GetTransform()->UpdateWorldMatrix();
	}
}

void SceneComponent::SetRotation(const XMVECTOR& rot)
{
	mTransformComponent->SetRotation(rot);
}

void SceneComponent::SetRotation(float x, float y, float z)
{
	mTransformComponent->SetRotation(x, y, z);
}

void SceneComponent::AdjustRotation(const XMVECTOR& rot)
{
	mTransformComponent->AdjustRotation(rot);
}

void SceneComponent::AdjustRotation(float x, float y, float z)
{
	mTransformComponent->AdjustRotation(x, y, z);
}

void SceneComponent::SetScale(const XMVECTOR& scale)
{
	mTransformComponent->SetScale(scale);
}

void SceneComponent::SetScale(float x, float y, float z)
{
	mTransformComponent->SetScale(x, y, z);
}

void SceneComponent::AdjustScale(const XMVECTOR& scale)
{
	mTransformComponent->AdjustScale(scale);
}

void SceneComponent::AdjustScale(float x, float y, float z)
{
	mTransformComponent->AdjustScale(x, y, z);
}

void SceneComponent::SetForwardVector(const XMVECTOR& relativeForwardVector)
{
	mTransformComponent->SetForwardVector(relativeForwardVector);
}

void SceneComponent::SetRightVector(const XMVECTOR& relativeRightVector)
{
	mTransformComponent->SetRightVector(relativeRightVector);
}

void SceneComponent::SetUpVector(const XMVECTOR& relativeUpVector)
{
	mTransformComponent->SetUpVector(relativeUpVector);
}

void SceneComponent::AttachToParent(SceneComponent* Parent)
{
	Parent->mChildren.push_back(this);
	
	mParent = Parent;
	mTransformComponent->SetParentTransform(Parent->GetTransform());
}