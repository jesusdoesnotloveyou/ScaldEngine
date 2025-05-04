Texture2D shaderTexture : register(t0);
// The depthMapTexture is the shadow map. This texture contains the scene depth buffer rendered from the light's perspective.
Texture2D depthMapTexture : register(t1);

struct VS_IN
{
    float4 inPosition : POSITION0;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct PS_IN
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    return output;
}