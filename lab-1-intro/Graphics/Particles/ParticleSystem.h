#pragma once

#include "../ScaldCoreTypes.h"

struct Particle
{
    XMVECTOR pos;	        // current position of the particle
    XMVECTOR prevPos;	    // last position of the particle
    XMVECTOR velocity;	    // direction and speed
    XMVECTOR acceleration;

    float energy;	        // determines how long the particle is alive
    float size;	            // size of particle
    float sizeDelta;	    // amount to change the size over time
    float weight;	        // determines how gravity affects the particle
    float weightDelta;      // change over time
    float color[4];	        // current color of the particle
    float colorDelta[4];    // how the color changes with time
};

class ParticleSystem 
{
public:
    ParticleSystem(int maxParticles, XMVECTOR origin);
    virtual void Update(float elapsedTime) = 0;
    virtual void Render() = 0;
    virtual int Emit(int numParticles);
    virtual void InitializeSystem();
    virtual ~ParticleSystem() noexcept;

protected:
    virtual void InitializeParticle(int index) = 0;

    Particle* particleList;     // particles for this emitter
    int       maxParticles;     // maximum number of particles in total
    int       numParticles;     // current number of particles
    XMVECTOR  origin;           // center of the particle system
    float     accumulatedTime;  // track when was last particle emitted
    XMVECTOR  force;            // force (gravity, wind, etc.) acting on the system
};