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

float3 CalcDirectionalLight(UniLight light, uniform float3 posW, uniform float3 normal, uniform float3 toEye)
{
    float3 lightDir = -light.direction;
    float3 ambient = light.ambient.xyz * light.ambient.w;
    float4 diffuse = light.diffuse;
    float4 specular = light.specular;
    
    // necessary vectors
    float3 viewDir = normalize(toEye - posW);
    float3 lightVector = normalize(lightDir);
    float3 reflectLight = normalize(reflect(-lightVector, normal));
    
    float3 diffuseLightIntensity = saturate(max(dot(lightVector, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 0.5f * pow(max(dot(reflectLight, viewDir), 0.0f), 200.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    return ambient * float3(0.6f, 0.6f, 0.6f) + diffuseLight + specularLight;
}

float3 CalcPointLight(UniLight light, uniform float3 posW, uniform float3 normal, uniform float3 toEye)
{    
    float4 diffuse = light.diffuse;
    float4 specular = light.specular;
    float3 lightPos = light.position;
    float3 attenuation = light.attenuation;
    
    // necessary vectors
    float3 V = normalize(toEye - posW);
    float3 L = -normalize(lightPos - posW);
    float3 R = normalize(reflect(L, normal));
    
    float3 diffuseLightIntensity = saturate(max(dot(L, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 0.8f * pow(max(dot(R, V), 0.0f), 200.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    float distanceToLight = distance(lightPos, posW);
    float attenuationFactor = pow(attenuation.x + attenuation.y * distanceToLight + attenuation.z * pow(distanceToLight, 2), -1);
    
    return (diffuseLight + specularLight) * attenuationFactor;
}

float3 CalcSpotLight()
{
    return float3(1.0f, 1.0f, 1.0f);

}

float4 main(PS_IN input) : SV_Target
{
    const int DIRECTIONAL = 1;
    const int POINT = 2;
    const int SPOT = 3;
    
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
    float4 diffuseColor = diffuseTexture.Load(input.inPosition.xyz);
    float3 normal = normalTexture.Load(input.inPosition.xyz);
    float3 pos = posTexture.Load(input.inPosition.xyz);
    
    uint width, height, elements, levels;
    shadowMaps.GetDimensions(0, width, height, elements, levels);
    
    [branch]
    if (Light.lightType == DIRECTIONAL)
    {
        appliedLight += CalcDirectionalLight(Light, pos, normal, gEyePos.xyz);
    }
    else
    {
        appliedLight += CalcPointLight(Light, pos, normal, gEyePos.xyz);
    }
    return diffuseColor * float4(appliedLight, 1.0f);
    
    //output.LightAccumulation = (ambient + emissive);
}