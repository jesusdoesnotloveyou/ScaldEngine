#include "stdafx.h"
#include "KatamariPlayerPawn.h"
#include "GameFramework/Components/SceneComponent.h"

using namespace Scald;

KatamariPlayerPawn::KatamariPlayerPawn()
    : Super()
{
    m_pawnMovementComponent = new KatamariMovementComponent(shared_from_this());
    m_jumpZ = m_pawnMovementComponent->GetJumpZ();
}

KatamariPlayerPawn::~KatamariPlayerPawn() noexcept
{
}

void KatamariPlayerPawn::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
    m_pawnMovementComponent->Tick(deltaTime);

    if (m_bIsFalling)
    {
        DoJump(deltaTime);

        if (XMVectorGetY(m_rootComponent->GetPosition()) <= 1.9f)
        {
            StopJumping();
        }
    }
}

bool KatamariPlayerPawn::IsPlayerPawn() const
{
    return true;
}

void KatamariPlayerPawn::Jump()
{
    m_bIsFalling = true;
}

void KatamariPlayerPawn::StopJumping()
{
    m_bIsFalling = false;
    m_jumpZ = m_pawnMovementComponent->GetJumpZ();
}

void KatamariPlayerPawn::DoJump(float deltaTime)
{
    m_rootComponent->AdjustPosition(XMVector3Cross(m_rootComponent->GetForwardVector(), m_rootComponent->GetRightVector()) * m_jumpZ * deltaTime);
    m_jumpZ -= 0.9f;
}