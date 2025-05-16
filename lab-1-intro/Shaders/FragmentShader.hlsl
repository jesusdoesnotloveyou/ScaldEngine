//cbuffer alphaBuffer : register(b1)
//{
//    float alpha;
//};

Texture2D objTexture : TEXTURE : register(t0);
Texture2DArray depthMapTextures : register(t1);

SamplerState objSamplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSamplerState : SAMPLER : register(s1);

struct Gbuffer
{
    float4 DiffuseSpec : SV_Target0;
    float3 WorldPos : SV_Target1;
    float3 Normal : SV_Target2;
};

struct PointLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 position;
    float range;
    float3 attenuation;
    float pad;
};

struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
};

StructuredBuffer<PointLight> PointLights : register(t2);
StructuredBuffer<DirectionalLight> DirectionalLights : register(t3);
//StructuredBuffer<PointLight> SpotLights : register(t4);

cbuffer cbPerFrame : register(b0)
{
    float4 gEyePos;
    float numPointLights;
    float numDirectionalLights;
    //float Ks = 0.1f;
    //float shininess = 200.0f;
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

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
    float3 inWorld : WORLD_POSITION;
    float4 inWorldView : POSITION;
    float4 inLightSpacePos : TEXCOORD1;
};

float SampleShadowMap(int layer, float3 uvw, float depth)
{
    return depthMapTextures.SampleCmp(shadowSamplerState, float3(uvw.xy, layer), depth).r;
}

float3 GetShadowCoords(int layer, float3 worldPos)
{
    float4 coords = mul(float4(worldPos, 1.0f), CascData.ViewProj[layer]);
    coords.xyz /= coords.w;
    coords.xy = coords.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    return coords.xyz;
}

float3 CalculatePointLight(PointLight light, uniform float3 posW, uniform float3 normal, uniform float3 toEye)
{
    // return vec init
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
     // from structured buffer
    float3 lightPos = light.position;
    float3 attenuation = light.attenuation;
    float4 ambient = light.ambient;
    float4 diffuse = light.diffuse;
    
    // necessary vectors
    float3 viewDir = normalize(toEye - posW);
    float3 lightVector = normalize(lightPos - posW);
    float3 reflectLight = normalize(reflect(-lightVector, normal));
    
    // dividing by numPointLights to avoid overexposure due to cumulative effect of ambient light
    // ambient component will be removed further (deffered shading)
    float3 ambientLight = ambient.xyz * (ambient.w / numPointLights);
    //float3 diffuseLightIntensity = (dot(lightVector, normal)); // for testing: more explicit view of light propagation
    float3 diffuseLightIntensity = saturate(max(dot(lightVector, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 1.2f * pow(max(dot(reflectLight, viewDir), 0.0f), 100.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    float distanceToLight = distance(lightPos, posW);
    float attenuationFactor = pow(attenuation.x + attenuation.y * distanceToLight + attenuation.z * pow(distanceToLight, 2), -1);
    
    appliedLight = ambientLight + (diffuseLight + specularLight) * attenuationFactor;
    return appliedLight;
}

float3 CalculateDirectionalLight(DirectionalLight light, uniform float3 posW, uniform float3 normal, uniform float3 toEye)
{
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
     // from structured buffer
    float3 lightDir = -light.direction;
    float4 ambient = light.ambient;
    float4 diffuse = light.diffuse;
    float4 specular = light.specular;
    
    // necessary vectors
    float3 viewDir = normalize(toEye - posW);
    float3 lightVector = normalize(lightDir);
    float3 reflectLight = normalize(reflect(-lightVector, normal));
    
    // overexposure if there are a number of directional light sources
    float3 ambientLight = ambient.xyz * ambient.w;
    float3 diffuseLightIntensity = saturate(max(dot(lightVector, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 10.0f * pow(max(dot(reflectLight, viewDir), 0.0f), 50.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    appliedLight = ambientLight + diffuseLight + specularLight;
    return appliedLight;
}

float4 main(PS_IN input) : SV_Target
{
    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
    int layer = 3;
    float viewDepth = abs(input.inWorldView.z / input.inWorldView.w);
    for (int i = 0; i < 4; ++i)
    {
        if (viewDepth < CascData.Distances[i])
        {
            layer = i;
            break;
        }
    }
    
    // Calculate the projected texture coordinates.
    float3 shadowTexCoords = GetShadowCoords(layer, input.inWorld);
    
    // Check if the pixel texture coordinate is in the view frustum of the light before doing any shadow work
    if (saturate(shadowTexCoords.x) == shadowTexCoords.x && saturate(shadowTexCoords.y) == shadowTexCoords.y)
    {
        float bias = max(0.005 * (1.0 - dot(input.inNormal, normalize(DirectionalLights[0].direction))), 0.001);
        float currentDepth = shadowTexCoords.z - 0.0005f;
    
        float shadow = SampleShadowMap(layer, shadowTexCoords, currentDepth);
        
        for (float j = 0; j < numDirectionalLights; j++)
        {
            appliedLight += CalculateDirectionalLight(DirectionalLights[j], input.inWorld, input.inNormal, gEyePos.xyz);
        }
        
        /*for (i = 0; i < numPointLights; i++)
        {
            appliedLight += CalculatePointLight(PointLights[i], input.inWorldPos, input.inNormal, gEyePos.xyz);
        }*/
        
        float3 finalColor = sampleColor.xyz * appliedLight;
        return float4(finalColor, 1.0f);
    }
    else
    {
        return float4(sampleColor.xyz * DirectionalLights[0].ambient.xyz, 1.0f);
    }
}