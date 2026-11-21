#ifdef __cplusplus

#include "DirectXMath.h"

using float4x4 = DirectX::XMMATRIX;
using float4   = DirectX::XMFLOAT4;
using float3   = DirectX::XMFLOAT3;
using float2   = DirectX::XMFLOAT2;
using uint     = uint32_t;

#endif

struct PerObjectConstants
{
    float4x4 World;
    float4x4 InvTransposeWorld;
};

struct PerFrameConstants
{
    float4x4 View;
    float4x4 Proj;
    float4x4 ViewProj;
    float3 EyePos;
    float _padding;
};