//cbuffer alphaBuffer : register(b1)
//{
//    float alpha;
//};

cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
    
    float3 dynamicLightColor;
    float dynamicLightStrength;
    float3 dynamicLightPosition;
    
    float attenuationConstantFactor;
    float attenuationLinearFactor;
    float attenuationExponentialFactor;
};

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

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);


float4 main(PS_IN input) : SV_Target
{
    float3 lightVector = normalize(dynamicLightPosition - input.inWorldPos);
    float3 reflectLight = normalize(reflect(-lightVector, input.inNormal));
    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    float3 viewDir = normalize(gEyePos.xyz - input.inWorldPos);
    
    //float3 sampleColor = input.inNormal;
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    
    //float3 diffuseLightIntensity = (dot(lightVector, input.inNormal)); // for testing: more explicit view of light propagation
    float3 diffuseLightIntensity = saturate(max(dot(lightVector, input.inNormal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;
    
    float3 specularIntensity = 10.f * pow(max(dot(reflectLight, viewDir), 0.0f), 20.0f);
    float3 specularLight = saturate(specularIntensity);
    
    float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
    float attenuationFactor = pow(attenuationConstantFactor + attenuationLinearFactor * distanceToLight + attenuationExponentialFactor * pow(distanceToLight, 2), -1);
    
    float3 appliedLight = ambientLight + (diffuseLight + specularLight) * attenuationFactor;
    float3 finalColor = sampleColor.xyz * appliedLight;
    
    return float4(finalColor.xyz, 1.0f);
}