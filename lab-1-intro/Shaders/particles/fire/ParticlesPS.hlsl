Texture2D billboardTexture: register(t0);
SamplerState particleSampler : SAMPLER : register(s0);

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD0;
    float4 inColor : COLOR0;
};

float4 main(PS_IN input) : SV_TARGET
{
    return input.inColor * billboardTexture.Sample(particleSampler, input.inTexCoord);
}