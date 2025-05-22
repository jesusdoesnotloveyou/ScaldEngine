cbuffer cbPerObject : register(b0)
{
    matrix gWorld;
    matrix gView;
    matrix gProjection;
};

struct VS_IN
{
    float4 inPosition : POSITION0;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD0;
    float3 outNormal : NORMAL;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    return output;
}