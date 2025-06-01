#pragma once

#include "ParticleSystem.h"

class FireParticles : ParticleSystem
{
public:
    FireParticles(int maxParticles, XMVECTOR origin);
    virtual void Update(float elapsedTime) override;
    virtual void Render() override;
    virtual int Emit(int numParticles) override;
    virtual void InitializeSystem() override;
    virtual ~FireParticles() noexcept override;

protected:
    virtual void InitializeParticle(int index) override;
};