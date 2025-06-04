#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>

#include <random>

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=nullptr; } }

using namespace DirectX;

struct Particle
{
    XMVECTOR pos            = XMVectorZero();
    XMVECTOR prevPos        = XMVectorZero();
    XMVECTOR velocity       = XMVectorZero();
    XMVECTOR acceleration   = XMVectorZero();
    XMVECTOR initialColor   = XMVectorZero();
    XMVECTOR endColor       = XMVectorZero();
    float maxLifeTime   = 1.0f;
    float lifeTime      = 0.0f;
    float initialSize   = 1.0f;
    float endSize       = 1.0f; // or sizeDelta
    float initialWeight = 1.0f;
    float endWeight     = 1.0f; // or weightDelta
    float pad[2];
};

struct ParticleConstantBuffer
{
    float deltaTime         = 0.0f;
    UINT maxNumParticles    = 0u;
    UINT numEmitInThisFrame = 0u;
    UINT numAliveParticles  = 0u;
    XMVECTOR gEmitPos       = XMVectorZero();
    XMVECTOR gEyePos        = XMVectorZero();
};

struct Camera—onstantBuffer
{
    XMMATRIX gView          = XMMatrixIdentity();
    XMMATRIX gProjection    = XMMatrixIdentity();
    XMVECTOR gForward       = XMVectorZero();
    XMVECTOR gUp            = XMVectorZero();
};

struct SortList
{
    UINT index = 0u;
    float distanceSq = std::numeric_limits<float>().max();
};

struct SortConstantBuffer
{
    UINT g_iLevel;
    UINT g_iLevelMask;
    UINT g_iWidth;
    UINT g_iHeight;
};

template<typename T>
T generateRandom(T min, T max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dist(min, max);
    return dist(gen);
}