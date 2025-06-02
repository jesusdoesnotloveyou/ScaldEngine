#pragma once

#include "ParticleSystem.h"

class FireParticleSystem final : ParticleSystem
{
public:
    FireParticleSystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int maxParticles, XMVECTOR origin);
    virtual void Update(float elapsedTime) override;
    virtual void Render() override;
    virtual int Emit(int numParticles) override;
    virtual void InitializeSystem() override;
    void InitializeParticles(int count);
    virtual ~FireParticleSystem() noexcept override;
protected:
    virtual void InitializeParticle(int index) override;

private:
    void Simulate(float elapsedTime);
    void SortParticles();

private:
    VertexShader mFireVertexShader;
    GeometryShader mFireGeometryShader;
    PixelShader mFirePixelShader;
    ComputeShader mFireSimulateShader;
    ComputeShader mFireEmitShader;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> mParticleSamplerClamp;
    ID3D11ShaderResourceView* mBillboardTexture = nullptr;
};