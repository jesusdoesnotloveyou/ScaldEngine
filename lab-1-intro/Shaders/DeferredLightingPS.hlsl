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
    matrix gView;
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

float3 CalcDirectionalLight(UniLight light, uniform float3 posW, uniform float3 normal, uniform float4 toEye)
{
    float3 lightDir = normalize(-light.direction);
    float3 ambient = light.ambient.xyz * light.ambient.w;
    float4 diffuse = light.diffuse;
    float4 specular = light.specular;
    
    // necessary vectors
    float3 viewDir = normalize(toEye.xyz - posW);
    float3 reflectLight = normalize(reflect(-lightDir, normal));
    
    float3 diffuseLightIntensity = saturate(max(dot(lightDir, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 0.5f * pow(max(dot(reflectLight, viewDir), 0.0f), 200.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    return diffuseLight + specularLight;
}

float3 CalcPointLight(UniLight light, uniform float3 posW, uniform float3 normal, uniform float4 toEye)
{    
    float4 diffuse = light.diffuse;
    float4 specular = light.specular;
    float3 lightPos = light.position;
    float3 attenuation = light.attenuation;
    
    // necessary vectors
    float3 V = normalize(toEye.xyz - posW);
    float3 L = normalize(lightPos - posW);
    float3 R = normalize(reflect(-L, normal));
    
    float3 diffuseLightIntensity = saturate(max(dot(L, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 0.8f * pow(max(dot(R, V), 0.0f), 200.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    float distanceToLight = distance(lightPos, posW);
    if (distanceToLight > light.range)
        return float3(0.0f, 0.0f, 0.0f);
    
    float attenuationFactor = (1.0f - distanceToLight / light.range);
    attenuationFactor *= attenuationFactor;
    return (diffuseLight + specularLight) * attenuationFactor;
}

void CalcSpotLight(UniLight L, float3 posW, float3 normal, float3 toEye, out float4 diffuse, out float4 spec)
{
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = L.position - posW;
    float d = length(lightVec);
    
    if(d > L.range ) return;
    
    lightVec /= d;
    
    float diffuseFactor = dot(lightVec, normal);

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), 200.0f);
        diffuse = diffuseFactor * L.diffuse;
        spec = specFactor * L.specular;
    }
    
    float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.spot);
    float att = spot / dot(L.attenuation, float3(1.0f, d, d * d));
    
    diffuse *= att;
    spec *= att;
}

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
    
    float3 shadowTexCoords = GetShadowCoords(layer, pos);
    
    float3 ambient = float3(0.0f, 0.0f, 0.0f);
    
    [branch]
    if (Light.lightType == DIRECTIONAL)
    {
        ambient = Light.ambient.xyz * Light.ambient.z;
        float shadow = 0.0f;
        if (saturate(shadowTexCoords.x) == shadowTexCoords.x && saturate(shadowTexCoords.y) == shadowTexCoords.y)
        {
            float currentDepth = shadowTexCoords.z - 0.002f;
    
            [unroll]
            for (int i = 0; i < 9; ++i)
            {
                shadow += SampleShadowMap(layer, shadowTexCoords.xy + offsets[i], currentDepth);
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