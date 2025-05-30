Texture2D gBufferTex : register(t0);
SamplerState texSampler : SAMPLER : register(s0);

struct PS_IN
{
    float4 inPosition : SV_POSITION0;
    float2 inTexCoord : TEXCOORD0;
};

float4 main(PS_IN input) : SV_TARGET
{
    return gBufferTex.Sample(texSampler, input.inTexCoord);
}