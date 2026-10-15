#include "LightCommon.hlsl"

Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);
Texture2D<float3> posTexture : register(t2);
Texture2DArray shadowMaps : register(t3);

SamplerState objSamplerState : SAMPLER: register(s0);
SamplerComparisonState shadowSamplerState : SAMPLER: register(s1);

struct CascadeData
{
    float4x4 ViewProj[4];
    float4 Distances; // not used, so not filled on the CPU side
};

cbuffer CascBuf : register(b0)
{
    CascadeData CascData;
}

float SampleShadowMap(int layer, float2 uv, float depth)
{
    return shadowMaps.SampleCmp(shadowSamplerState, float3(uv, layer), depth).r;
}

float3 GetCascadeCoords(int layer, float3 worldPos)
{
    float4 coords = mul(float4(worldPos, 1.0f), CascData.ViewProj[layer]);
    coords.xyz /= coords.w;
    coords.xy = coords.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    return coords.xyz;
}

cbuffer cbPerFrame : register(b1)
{
    float4x4 gView;
    float4x4 gProjection;
    float4x4 gViewProj;
    float4 gEyePos;
}

cbuffer LightCB : register(b2)
{
    UniLight Light;
}

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

float4 main(PS_IN input) : SV_Target
{
    const int DIRECTIONAL = 1;
    const int POINT = 2;
    const int SPOT = 3;

    float4 diffuseColor = diffuseTexture.Load(input.inPosition.xyz);
    float3 normal = normalTexture.Load(input.inPosition.xyz);
    float3 pos = posTexture.Load(input.inPosition.xyz);

    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);

    int layer = 3;
    float4 worldView = mul(float4(pos, 1.0f), gView);

    float viewDepth = abs(worldView.z);
    for (int i = 0; i < 4; ++i)
    {
        if (viewDepth < CascData.Distances[i])
        {
            layer = i;
            break;
        }
    }

    uint width, height, elements, levels;
    shadowMaps.GetDimensions(0, width, height, elements, levels);

    const float dx = 1.0f / width;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };
    
    float3 cascadeTexCoords = GetCascadeCoords(layer, pos);
    
    float3 ambient = float3(0.0f, 0.0f, 0.0f);
    
    [branch]
    if (Light.lightType == DIRECTIONAL)
    {
        ambient = Light.ambient.xyz * Light.ambient.z;
        float shadow = 0.0f;
        if (saturate(cascadeTexCoords.x) == cascadeTexCoords.x && saturate(cascadeTexCoords.y) == cascadeTexCoords.y)
        {
            float currDepth = cascadeTexCoords.z - 0.0005f;
            [unroll]
            for (int i = 0; i < 9; ++i)
            {
                shadow += SampleShadowMap(layer, cascadeTexCoords.xy + offsets[i], currDepth);
            }
            shadow = shadow / 9.0f;
        }
        appliedLight += CalcDirectionalLight(Light, pos, normal, gEyePos) * shadow;
    }
    else
    {
        appliedLight += CalcPointLight(Light, pos, normal, gEyePos);
    }
    return diffuseColor * (float4(appliedLight, 1.0f) + float4(ambient, 1.0f));
}