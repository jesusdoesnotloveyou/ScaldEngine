#pragma once

#include "GameFramework/Objects/Pawn.h"
#include "KatamariMovementComponent.h"

namespace Scald
{
    class KatamariPlayerPawn : public Pawn
    {
        using Super = Pawn;
    public:
        KatamariPlayerPawn();
        virtual ~KatamariPlayerPawn() noexcept override;

        virtual void Tick(float deltaTime) override;
        virtual bool IsPlayerPawn() const;

    public:
        void Jump();
        void StopJumping();
        void DoJump(float deltaTime);

        FORCEINLINE bool IsFalling() { return m_bIsFalling; }
        
    private:
        bool m_bIsFalling = false;
        float m_jumpZ = 0.0f;
    };
}