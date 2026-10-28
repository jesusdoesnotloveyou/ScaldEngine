// Structure for representing every type of light in deferred shading
// TODO: consider using different (dx12 renderer) structs that are below
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
    int lightType;
};

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
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 position;
    float range;
    float3 attenuation;
    float pad;
};

struct SpotLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 position;
    float range;
    float3 direction;
    float spot;
    float3 attenuation;
    float pad;
};

float3 CalcDirectionalLight(UniLight light, uniform float3 posW, uniform float3 normal, uniform float4 toEye)
{
    float3 lightDir = normalize(-light.direction);
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
    float3 pointLight = float3(0.0f, 0.0f, 0.0f);
    
    float4 diffuse = light.diffuse;
    float4 specular = light.specular;
    float3 lightPos = light.position;
    //float3 attenuation = light.attenuation;

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
        return pointLight;

    float attenuationFactor = (1.0f - distanceToLight / light.range);
    attenuationFactor *= attenuationFactor;
    pointLight = (diffuseLight + specularLight) * attenuationFactor;
    
    return pointLight;
}

void CalcSpotLight(UniLight L, float3 posW, float3 normal, float3 toEye, out float4 diffuse, out float4 spec)
{
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightVec = L.position - posW;
    float d = length(lightVec);

    if (d > L.range)
        return;

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