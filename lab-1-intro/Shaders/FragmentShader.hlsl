//cbuffer alphaBuffer : register(b1)
//{
//    float alpha;
//};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

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

StructuredBuffer<PointLight> Lights : register(t1);

cbuffer cbPerFrame : register(b1)
{
    float4 gEyePos;
    //float Ks = 0.1f;
    //float shininess = 200;
};

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos: WORLD_POSITION;
};

float4 main(PS_IN input) : SV_Target
{
    // from structured buffer
    float3 lightPos = Lights[0].position;
    float3 attenuation = Lights[0].attenuation;
    float4 ambient = Lights[0].ambient;
    float4 diffuse = Lights[0].diffuse;
    //
    
    float3 lightVector = normalize(lightPos - input.inWorldPos);
    float3 reflectLight = normalize(reflect(-lightVector, input.inNormal));
    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    float3 viewDir = normalize(gEyePos.xyz - input.inWorldPos);
    
    //float3 sampleColor = input.inNormal;
    float3 ambientLight = ambient.xyz * ambient.w;
    
    //float3 diffuseLightIntensity = (dot(lightVector, input.inNormal)); // for testing: more explicit view of light propagation
    float3 diffuseLightIntensity = saturate(max(dot(lightVector, input.inNormal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 10.f * pow(max(dot(reflectLight, viewDir), 0.0f), 20.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    float distanceToLight = distance(lightPos, input.inWorldPos);
    float attenuationFactor = pow(attenuation.x + attenuation.y * distanceToLight + attenuation.z * pow(distanceToLight, 2), -1);
    
    float3 appliedLight = ambientLight + (diffuseLight + specularLight) * attenuationFactor;
    //float3 appliedLight = ambientLight + (diffuseLight + specularLight);
    float3 finalColor = sampleColor.xyz * appliedLight;
    
    return float4(finalColor, 1.0f);
}