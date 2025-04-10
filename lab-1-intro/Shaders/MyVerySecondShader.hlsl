//cbuffer alphaBuffer : register(b1)
//{
//    float alpha;
//};

cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
};

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_Target
{
    float4 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);

    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 finalColor = sampleColor.xyz * ambientLight;
    
    return float4(finalColor.xyz, 1.0f);
}