
Texture2D objTexture : TEXTURE : register(t0);

SamplerState objSamplerState : SAMPLER : register(s0);

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
};

struct Gbuffer
{
    float4 DiffuseSpec : SV_Target0;
    float3 Normal : SV_Target1;
    float3 WorldPos : SV_Target2;
};

[earlydepthstencil]
Gbuffer main(PS_IN input)
{
    Gbuffer output = (Gbuffer) 0;

    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    output.DiffuseSpec = sampleColor;
    
    output.Normal = input.inNormal;
    
    output.WorldPos = float3(1.0f, 1.0f, 1.0f);

    return output;
}