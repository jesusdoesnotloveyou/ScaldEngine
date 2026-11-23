// Common vertex shader for spot and point lights

#include "Common.hlsli"
#include "LightCommon.hlsli"

Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);
Texture2D<float3> posTexture : register(t2);

StructuredBuffer<PointLight> pointLights : register(t3);
StructuredBuffer<SpotLight> spotLights : register(t4);

cbuffer cbPerFrame : register(b1)
{
    float4x4 gView;
    float4x4 gProjection;
    float4x4 gViewProj;
    float4 gEyePos;
}

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
    nointerpolation uint iInstanceID : InstanceID;
};

float4 main(VS_OUT input)
{
    float3 normal = normalTexture.Load(input.outPosition.xyz);
    float3 pos = posTexture.Load(input.outPosition.xyz);

    PointLight Light = pointLights[input.iInstanceID];

    float3 appliedLight = CalcPointLight(Light, pos, normal, gEyePos);

    return float4(appliedLight, 1.0f);
}