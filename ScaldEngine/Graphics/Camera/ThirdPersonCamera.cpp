#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "GameFramework/Objects/Actor.h"

using namespace Scald;

ThirdPersonCamera::ThirdPersonCamera()
{
    mPitch = 0.0f;
    mYaw = XM_PIDIV4;
}

ThirdPersonCamera::ThirdPersonCamera(std::shared_ptr<Actor> owner)
    : Camera(owner)
    , mPitch(0.0f)
    , mYaw(0.0f)
{

}

std::shared_ptr<SceneComponent> ThirdPersonCamera::GetTarget() const
{
    return m_target;
}

void ThirdPersonCamera::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    // TODO: uncomment when scene component will be finished
    //UpdatePosition();
}

void ThirdPersonCamera::UpdatePosition()
{
    auto anchorPosition = m_target->GetPosition();
    // Update position: spherical coordinates to Cartesian
    const float newX = XMVectorGetX(anchorPosition) + mArmLength * cosf(mPitch) * sinf(mYaw);
    const float newY = XMVectorGetY(anchorPosition) + mArmLength * sinf(mPitch);
    const float newZ = XMVectorGetZ(anchorPosition) + mArmLength * cosf(mPitch) * cosf(mYaw);
    SetPosition(XMVectorSet(newX, newY, newZ, 0.0f));

    // Update Rotation
    SetLookAtPosition(anchorPosition);
}

void ThirdPersonCamera::AdjustRotation(float x, float y, float z)
{
    mPitch += x;
    if (mPitch > XM_PIDIV2 - 0.1f) mPitch = XM_PIDIV2 - 0.1f;
    if (mPitch < -XM_PIDIV2 + 0.1f) mPitch = -XM_PIDIV2 + 0.1f;
    mYaw += y;
}