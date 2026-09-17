#include "stdafx.h"

#include "Pawn.h"
#include "GameFramework/Components/Movement/MovementComponent.h"

using namespace Scald;

Pawn::Pawn()
{
    //m_pawnMovementComponent = CreateComponent<MovementComponent>();
    
    // m_bCastsShadow = true;
}

Pawn::~Pawn() noexcept
{
    if (m_pawnMovementComponent) delete m_pawnMovementComponent;
}

void Pawn::Tick(float deltaTime) {}

MovementComponent* Pawn::GetMovement() const
{
    return m_pawnMovementComponent;
}