#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int maxParticles, XMVECTOR origin)
	: 
	mDevice(device),
	mDeviceContext(deviceContext),
	maxParticles(maxParticles),
	origin(origin)
{
	particleList.reserve(maxParticles);
}

int ParticleSystem::Emit(int numParticles)
{
	return 0;
}

void ParticleSystem::InitializeSystem()
{
	ThrowIfFailed(bitonicSortShader.Init(mDevice, L"./Shaders/SortCS.hlsl"));
	ThrowIfFailed(bitonicTransposeShader.Init(mDevice, L"./Shaders/SortTransposeCS.hlsl"));

	//ThrowIfFailed(CreateRWStructuredBuffer(mDevice, &particelBuffer, particleList));
}

ParticleSystem::~ParticleSystem() noexcept
{
}