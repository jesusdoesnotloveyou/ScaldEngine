//cbuffer alphaBuffer : register(b1)
//{
//    float alpha;
//};

Texture2D objTexture : TEXTURE : register(t0);
//The depthMapTexture is the shadow map. This texture contains the scene depth buffer rendered from the light's perspective.
Texture2D depthMapTexture : TEXTURE : register(t1);
//Texture2DArray depthMapTextures : TEXTURE : register(t1);

SamplerState objSamplerState : SAMPLER : register(s0);
SamplerState shadowSamplerState : SAMPLER : register(s1);

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

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
    float3 inWorldPos: WORLD_POSITION;
    float4 inLightSpacePos : TEXCOORD1;
};

float3 CalculatePointLight(PointLight light, uniform float3 posW, uniform float3 normal, uniform float3 toEye)
{
    // return vec init
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
     // from structured buffer
    float3 lightPos = light.position;
    float3 attenuation = light.attenuation;
    float4 ambient = light.ambient;
    float4 diffuse = light.diffuse;
    //
    
    // necessary vectors
    float3 viewDir = normalize(toEye - posW);
    float3 lightVector = normalize(lightPos - posW);
    float3 reflectLight = normalize(reflect(-lightVector, normal));
    
    // dividing by numPointLights to avoid overexposure due to cumulative effect of ambient light
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
    // return vec init
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
     // from structured buffer
    float3 lightDir = -light.direction;
    float4 ambient = light.ambient;
    float4 diffuse = light.diffuse;
    float4 specular = light.specular;
    //
    
    // necessary vectors
    float3 viewDir = normalize(toEye - posW);
    float3 lightVector = normalize(lightDir);
    float3 reflectLight = normalize(reflect(-lightVector, normal));
    
    // overexposure if there are some directional light sources
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
    //float3 sampleColor = input.inNormal; 
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
    // Calculate the projected texture coordinates.
    float2 shadowTexCoords;
    // dividing by w isn't necessary with an orthographic projection
    shadowTexCoords.x = 0.5f + (input.inLightSpacePos.x / input.inLightSpacePos.w * 0.5f);
    shadowTexCoords.y = 0.5f - (input.inLightSpacePos.y / input.inLightSpacePos.w * 0.5f);
    
    // Check if the pixel texture coordinate is in the view frustum of the light before doing any shadow work
    if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) && (saturate(shadowTexCoords.y) == shadowTexCoords.y))
    {
        // Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location
        float depthValue = depthMapTexture.Sample(shadowSamplerState, shadowTexCoords).r;
        // Calculate the depth of the light.
        float lightDepthValue = input.inLightSpacePos.z / input.inLightSpacePos.w;
    
        // Subtract the bias from the lightDepthValue.
        lightDepthValue = lightDepthValue - 0.00005f;
        
        // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
        
        if (lightDepthValue < depthValue)
        {
            // Directional Lights
            for (float i = 0; i < numDirectionalLights; i++)
            {
                appliedLight += CalculateDirectionalLight(DirectionalLights[i], input.inWorldPos, input.inNormal, gEyePos.xyz);
            }
            
            // Point Lights
            //for (i = 0; i < numPointLights; i++)
            //{
            //    appliedLight += CalculatePointLight(PointLights[i], input.inWorldPos, input.inNormal, gEyePos.xyz);
            //}
        }
        else
        {
            appliedLight = DirectionalLights[0].ambient.xyz;
        }
    }
    // Can comment out this else clause to see exactly where your shadow map range begins and ends
    else
    {
        // If this is outside the area of shadow map range then draw things normally with regular lighting.
        // ambient + diffuse
    }
   
    float3 finalColor = sampleColor.xyz * appliedLight;
    return float4(finalColor, 1.0f);
}