#pragma once

#include "GameFramework/Objects/Actor.h"

namespace Scald
{
    class MovementComponent;
    
    class Pawn : public Actor
    {
    public:
        Pawn();       
        ~Pawn() noexcept;

        virtual void Tick(float deltaTime) override;
        
        virtual MovementComponent* GetMovement() const;
        
        bool IsPlayer() const { return m_bIsPlayerPawn; }

    protected:
        MovementComponent* m_pawnMovementComponent;
        bool m_bIsPlayerPawn = false;
    };
}