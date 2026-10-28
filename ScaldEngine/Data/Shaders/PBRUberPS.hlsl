// For PerObject and PerFrame constants
#include "Common.hlsli"

Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);
Texture2D<float3> metallicTexture : register(t2);
Texture2D<float3> roughnessTexture : register(t3);
Texture2D<float3> ambientTexture : register(t4);

SamplerState texSampler : SAMPLER : register(s0);

// https://learnopengl.com/PBR/Theory#:~:text=previous%20lighting%20chapters.-,The%20reflectance%20equation,-This%20brings%20us
/*
    1. Microfacet model
    2. Energy conserving
    3. BRDF/BSDF/BTDF

    Fr = Kd*F_lambertian + Ks*F_cook-torr
    F_cook-torr = (D*F*G) / (4 * dot(wo,n) * dot(wi,n)) 

    D - normal distribution function
    F - Fresnel approximation for refr/refl light ratio
    G - geometry term (visibility and self-shadowing) 
*/

// Trowbridge-Reitz GGX normal distribution function
// H - halfway vector (either V and N or V and L)
float DistributionGGX(float3 N, float3 H, float a)
{
    float a2 = a*a;
    float NdotH = max(dot(N,H), 0.0f);
    float NdotH2 = NdotH*NdotH;

    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return a2 / denom;    
}

float RoughnessRemap(float roughness)
{
#ifdef IBL    
    return (roughness + 1.0f) * (roughness + 1.0f) / 8.0f;
#else // Direct lighting
    return roughness * roughness / 2.0f;
#endif
}

float GeometrySchlickGGX(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0f - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    return ggx1 * ggx2;
}

// cosTheta - angle between view direction and halfway (or normal) vector
// F0 - base reflectivity at normal incidence (usually 0.04 for dielectrics, varies for metals)
/*
    float3 F0 = float3(0.04f);
    F0 = lerp(F0, albedo, metallic);
*/
float FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

struct PS_IN
{
    float4 iPos       : SV_Position;
    float3 iNormal    : NORMAL;
    float2 iTexCoord  : TEXCOORD;
};

float4 main(PS_IN input) : SV_Target
{
    // Suppose that albedo texture is always exists

    float4 albedo = diffuseTexture.Sample(texSampler, input.iTexCoord);

#ifdef USE_NORMAL_MAP
    float3 normal = normalTexture.Sample(texSampler, input.iTexCoord);
#else
    float3 normal = input.iNormal;
#endif
#ifdef USE_METALLIC_MAP
    float metallic = metallicTexture.Sample(texSampler, input.iTexCoord).r;
#else
    float metallic = 0.04f;
#endif
#ifdef USE_ROUGHNESS_MAP
    float roughness = roughnessTexture.Sample(texSampler, input.iTexCoord).r;
#else
    float roughness = 0.5f;
#endif
#ifdef USE_AO_MAP
    float ambientOcclusion = ambientTexture.Sample(texSampler, input.iTexCoord).r;
#endif

    // Building the reflectance equation
    

    return float(1.0f).rrrr;
}