struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_Target
{
    float4 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);

    return pixelColor;
}