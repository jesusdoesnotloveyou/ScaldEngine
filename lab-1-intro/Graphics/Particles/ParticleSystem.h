#pragma once

#include "../../ScaldException.h"
#include "../ScaldCoreTypes.h"
#include "../Shaders.h"

#include <d3d11.h>
#include <wrl.h>

#include <vector>

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

struct SortConstantBuffer
{
    UINT g_iLevel;
    UINT g_iLevelMask;
    UINT g_iWidth;
    UINT g_iHeight;
};

class ParticleSystem 
{
public:
    ParticleSystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int maxParticles, XMVECTOR origin);
    virtual void Update(float elapsedTime) = 0;
    virtual void Render() = 0;
    virtual int Emit(int numParticles);
    virtual void InitializeSystem();
    virtual ~ParticleSystem() noexcept;

protected:

    template<typename T>
    HRESULT CreateRWStructuredBuffer(ID3D11Device* device, ID3D11Buffer** buffer, const std::vector<T>& bufferData)
    {
        UINT stride = (UINT)sizeof(T);
        UINT byteWidth = stride * (UINT)bufferData.size();

        D3D11_BUFFER_DESC desc;
        desc.ByteWidth = byteWidth;
        desc.Usage = D3D11_USAGE_DEFAULT; // without CPU write
        desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.StructureByteStride = stride;

        if (bufferData.data())
        {
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = bufferData.data();
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            return device->CreateBuffer(&desc, &data, buffer);
        }

        return device->CreateBuffer(&desc, nullptr, buffer);
    }

    template<typename T>
    HRESULT CreateRWStructuredBufferCPUWrite(ID3D11Device* device, ID3D11Buffer** buffer, const std::vector<T>& bufferData)
    {
        UINT stride = (UINT)sizeof(T);
        UINT byteWidth = stride * (UINT)bufferData.size();

        D3D11_BUFFER_DESC desc;
        desc.ByteWidth = byteWidth;
        desc.Usage = D3D11_USAGE_DYNAMIC; // without CPU write
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.StructureByteStride = stride;

        if (bufferData.data())
        {
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = bufferData.data();
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            return device->CreateBuffer(&desc, &data, buffer);
        }

        return device->CreateBuffer(&desc, nullptr, buffer);
    }

    virtual void InitializeParticle(int index) = 0;

    // Graphics context. Graphics object manages these resources.
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mDeviceContext = nullptr;

    std::vector<Particle> particleList;     // particles for this emitter
    int       maxParticles;     // maximum number of particles in total
    int       numParticles;     // current number of particles
    XMVECTOR  origin;           // center of the particle system
    float     accumulatedTime;  // track when was last particle emitted
    XMVECTOR  force;            // force (gravity, wind, etc.) acting on the system

    ID3D11Buffer* particelBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> particleBufferUAV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> particleBufferSRV;

    ID3D11Buffer* sortListBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> sortListBufferUAV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sortListBufferSRV;

    ID3D11Buffer* sortListBuffer2 = nullptr;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> sortListBufferUAV2;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sortListBufferSRV2;
    
    ID3D11Buffer* deadListBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> deadListBufferUAV;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> deadListBufferSRV;
    
    ID3D11Buffer* indirectArgsBuffer = nullptr;
    ID3D11Buffer* constSortBuf = nullptr;
     
    ComputeShader bitonicSortShader;
    ComputeShader bitonicTransposeShader;

    SortConstantBuffer mSortData;
};