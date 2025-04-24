Texture2D shaderTexture : register(t0);
// The depthMapTexture is the shadow map. This texture contains the scene depth buffer rendered from the light's perspective.
Texture2D depthMapTexture : register(t1);

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

float4 main(PS_IN input) : SV_TARGET
{
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}