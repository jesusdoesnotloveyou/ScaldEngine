
Texture2D objTexture : TEXTURE : register(t0);

SamplerState objSamplerState : SAMPLER : register(s0);

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
};

struct PS_OUT
{
    float4 outColor : SV_Target0;
    float3 outNormal : SV_Target1;
    float3 outWorldPos : SV_Target1;
};

PS_OUT main(PS_IN input) : SV_TARGET
{
    PS_OUT output = (PS_OUT) 0;

    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    output.outColor = sampleColor;
    
    output.outNormal = input.inNormal;
    
    output.outWorldPos = float3(1.0f, 1.0f, 1.0f);

    return output;
}