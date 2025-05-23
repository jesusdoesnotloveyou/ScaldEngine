Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);
Texture2D<float3> posTexture : register(t2);

SamplerComparisonState shadowSamplerState : SAMPLER : register(s0);

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

float4 main(PS_IN input) : SV_Target
{
    float4 diffuseColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
    float3 pos = float3(0.0f, 0.0f, 0.0f);
    
    diffuseColor = diffuseTexture.Load(input.inPosition.xyz);
    normal = normalTexture.Load(input.inPosition.xyz);
    pos = posTexture.Load(input.inPosition.xyz);
    
    //output.LightAccumulation = (ambient + emissive);
    //output.Diffuse = diffuse;
    //output.Specular = float4(specular.rgb, log2(specularPower) / 10.5f);
    //output.NormalVS = N;
 
    return diffuseColor;
}