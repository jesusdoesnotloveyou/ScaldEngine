#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	mLocalMatrix = mWorldMatrix = XMMatrixIdentity();
}

void TransformComponent::SetWorldMatrix(const XMMATRIX& worldMat)
{
	mWorldMatrix = worldMat;
}

void TransformComponent::Reset()
{
	mLocalMatrix = mWorldMatrix = XMMatrixIdentity();
}