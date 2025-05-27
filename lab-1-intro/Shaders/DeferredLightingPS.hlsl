Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);
Texture2D<float3> posTexture : register(t2);
Texture2DArray shadowMaps : register(t3);

SamplerState objSamplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSamplerState : SAMPLER : register(s1);

struct CascadeData
{
    matrix ViewProj[4];
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

float3 GetShadowCoords(int layer, float3 worldPos)
{
    float4 coords = mul(float4(worldPos, 1.0f), CascData.ViewProj[layer]);
    coords.xyz /= coords.w;
    coords.xy = coords.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    return coords.xyz;
}

cbuffer ConstantBufferData : register(b1)
{
    float4 gEyePos;
}

struct UniLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float spot;
    float3 attenuation;
    float range;
    float3 position;
    float pad;
    int lightType;
    float3 pad2;
};

cbuffer LightCB : register(b2)
{
    UniLight Light;
}

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

float3 CalculateDirectionalLight(UniLight light, float4 diffuse, uniform float3 posW, uniform float3 normal, uniform float3 toEye)
{
    float3 lightDir = -light.direction;
    float4 specular = light.specular;
    
    // necessary vectors
    float3 viewDir = normalize(toEye - posW);
    float3 lightVector = normalize(lightDir);
    float3 reflectLight = normalize(reflect(-lightVector, normal));
    
    float3 diffuseLightIntensity = saturate(max(dot(lightVector, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 0.5f * pow(max(dot(reflectLight, viewDir), 0.0f), 200.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    return diffuseLight + specularLight;
}

float4 main(PS_IN input) : SV_Target
{
    const int DIRECTIONAL = 1;
    const int POINT = 1;
    const int SPOT = 2;
    
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
    float4 diffuseColor = diffuseTexture.Load(input.inPosition.xyz);
    float3 normal = normalTexture.Load(input.inPosition.xyz);
    float3 pos = posTexture.Load(input.inPosition.xyz);
    
    uint width, height, elements, levels;
    shadowMaps.GetDimensions(0, width, height, elements, levels);
    
    if (Light.lightType == DIRECTIONAL)
    {
        appliedLight += CalculateDirectionalLight(Light, diffuseColor, pos, normal, gEyePos.xyz);
    }
    //output.LightAccumulation = (ambient + emissive);
    //output.Specular = float4(specular.rgb, log2(specularPower) / 10.5f);
 
    return float4(appliedLight, 1.0f);
}