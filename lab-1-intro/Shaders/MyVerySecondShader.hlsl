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
    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    //float3 sampleColor = input.inNormal;
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 appliedLight = ambientLight;
    float3 lightVector = normalize(dynamicLightPosition - input.inWorldPos);
    
    float3 diffuseLightIntensity = /*saturate*/(max(dot(lightVector, input.inNormal), 0.0f));
    // for testing: more explicit view of light
    //float3 diffuseLightIntensity = (dot(lightVector, input.inNormal));
    
    // distance to pixel from light source
    float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);
    
    float attenuationFactor = pow(attenuationConstantFactor + attenuationLinearFactor * distanceToLight + attenuationExponentialFactor * pow(distanceToLight, 2), -1);
    diffuseLightIntensity *= attenuationFactor;
    
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;
    appliedLight += diffuseLight;
    float3 finalColor = sampleColor.xyz * appliedLight;
    
    return float4(finalColor.xyz, 1.0f);
}