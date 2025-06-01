#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int maxParticles, XMVECTOR origin)
{
}

int ParticleSystem::Emit(int numParticles)
{
	return 0;
}

void ParticleSystem::InitializeSystem()
{
}

ParticleSystem::~ParticleSystem() noexcept
{
	if (particleList) delete[] particleList;
}