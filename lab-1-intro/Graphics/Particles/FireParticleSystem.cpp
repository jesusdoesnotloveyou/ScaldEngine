#include "FireParticleSystem.h"
#include <WICTextureLoader.h>

#include <cmath>

FireParticleSystem::FireParticleSystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int maxParticles, XMVECTOR origin)
	: ParticleSystem(device, deviceContext, maxParticles, origin)
{

}

void FireParticleSystem::Update(float elapsedTime)
{
	Emit(std::round(12000 * elapsedTime));
	Simulate(elapsedTime);
	SortParticles();
}

void FireParticleSystem::Render()
{

}

int FireParticleSystem::Emit(int numParticles)
{
	InitializeParticles(numParticles);
	return 0;
}

void FireParticleSystem::InitializeSystem()
{
	ParticleSystem::InitializeSystem();

	//ThrowIfFailed(mFireVertexShader.Init(mDevice, nullptr, 0u, L""));
	ThrowIfFailed(mFireGeometryShader.Init(mDevice, L"./Shaders/BillboardParticleGS.hlsl"));
	//ThrowIfFailed(mFirePixelShader.Init(mDevice, L""));
	ThrowIfFailed(mFireSimulateShader.Init(mDevice, L"./Shaders/SimulateCS.hlsl"));
	ThrowIfFailed(mFireEmitShader.Init(mDevice, L"./Shaders/EmitCS.hlsl"));


	D3D11_SAMPLER_DESC sampleDesc = {};
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampleDesc.BorderColor[0] = 1.0f;
	sampleDesc.BorderColor[1] = 0.0f;
	sampleDesc.BorderColor[2] = 0.0f;
	sampleDesc.BorderColor[3] = 1.0f;
	sampleDesc.MipLODBias = 0;
	sampleDesc.MaxAnisotropy = 0;
	sampleDesc.MaxLOD = INT_MAX;
	ThrowIfFailed(mDevice->CreateSamplerState(&sampleDesc, mParticleSamplerClamp.GetAddressOf()));

	ThrowIfFailed(CreateWICTextureFromFile(mDevice, L"./Data/Textures/pop_cat_color.png", nullptr, &mBillboardTexture));
}

FireParticleSystem::~FireParticleSystem() noexcept
{
}

void FireParticleSystem::InitializeParticle(int index)
{

}

void FireParticleSystem::Simulate(float elapsedTime)
{

}

void FireParticleSystem::SortParticles()
{

}

void FireParticleSystem::InitializeParticles(int count)
{
	for (int i = 0; i < count; i++)
	{
		InitializeParticle(i);
	}
}