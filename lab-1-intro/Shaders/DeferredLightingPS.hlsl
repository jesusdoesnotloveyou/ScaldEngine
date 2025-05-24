struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
};

struct PointLight
{
    float4 diffuse;
    float4 specular;
    float3 position;
    float range;
    float3 attenuation;
    float pad;
};

struct SpotLight
{
    float4 diffuse;
    float4 specular;
    float3 position;
    float range;
    float3 direction;
    float spot;
    float3 attenuation;
    float pad;
};

cbuffer LightWCB : register(b0)
{
    DirectionalLight dirLight;
    float numPointLights;
    float numSpotLights;
};

struct CascadeData
{
    matrix ViewProj[4];
    float4 Distances; // not used, so not filled on the CPU side
};

cbuffer CascBuf : register(b1)
{
    CascadeData CascData;
}

Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);
Texture2D<float3> posTexture : register(t2);

Texture2DArray shadowMaps : register(t3);

StructuredBuffer<PointLight> PointLights : register(t4);
StructuredBuffer<SpotLight> SpotLights : register(t5);

SamplerComparisonState shadowSamplerState : SAMPLER : register(s0);

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

float4 main(PS_IN input) : SV_Target
{
    float4 diffuseColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
    float3 pos = float3(0.0f, 0.0f, 0.0f);
    
    diffuseColor = diffuseTexture.Load(input.inPosition.xyz);
    normal = normalTexture.Load(input.inPosition.xyz);
    pos = posTexture.Load(input.inPosition.xyz);
    
    //output.LightAccumulation = (ambient + emissive);
    //output.Diffuse = diffuse;
    //output.Specular = float4(specular.rgb, log2(specularPower) / 10.5f);
    //output.NormalVS = N;
 
    uint width, height, elements, levels;
    shadowMaps.GetDimensions(0, width, height, elements, levels);
    
    return diffuseColor + dirLight.ambient.x * levels * pos.x * normal.x * PointLights[0].pad * SpotLights[0].pad;
}