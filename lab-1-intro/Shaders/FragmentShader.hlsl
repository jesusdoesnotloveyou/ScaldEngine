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

// Point light
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
    float numLights;
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

float3 CalculateLight(PointLight light, uniform float3 posW, uniform float3 normal, uniform float3 toEye)
{
    // return vec init
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
     // from structured buffer
    float3 lightPos = light.position;
    float3 attenuation = light.attenuation;
    float4 ambient = light.ambient;
    float4 diffuse = light.diffuse;
    //
    
    float3 viewDir = normalize(toEye - posW);
    float3 lightVector = normalize(lightPos - posW);
    float3 reflectLight = normalize(reflect(-lightVector, normal));
    
    // dividing by numLights to avoid overexposure due to cumulative effect of ambient light
    float3 ambientLight = ambient.xyz * (ambient.w / numLights);
    //float3 diffuseLightIntensity = (dot(lightVector, normal)); // for testing: more explicit view of light propagation
    float3 diffuseLightIntensity = saturate(max(dot(lightVector, normal), 0.0f));
    float3 diffuseLight = diffuseLightIntensity * diffuse.xyz * diffuse.w;
    
    float3 specularIntensity = 10.f * pow(max(dot(reflectLight, viewDir), 0.0f), 20.0f); // * specular.xyz
    float3 specularLight = saturate(specularIntensity);
    
    float distanceToLight = distance(lightPos, posW);
    float attenuationFactor = pow(attenuation.x + attenuation.y * distanceToLight + attenuation.z * pow(distanceToLight, 2), -1);
    
    appliedLight = ambientLight + (diffuseLight + specularLight) * attenuationFactor;
    return appliedLight;
}

float4 main(PS_IN input) : SV_Target
{
    // necessary vectors
    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    //float3 sampleColor = input.inNormal;
  
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
    for (float i = 0; i < numLights; i++)
    {
        appliedLight += CalculateLight(Lights[i], input.inWorldPos, input.inNormal, gEyePos.xyz);
    }    
    
    float3 finalColor = sampleColor.xyz * appliedLight;
    
    return float4(finalColor, 1.0f);
}