cbuffer cbPerObject : register(b0)
{
    matrix gWorldViewProj;
    matrix gWorld;
};

cbuffer CBufChangeOnResize : register(b1)
{
    matrix mProjection;
}

struct VS_IN
{
    float4 inPosition : POSITION0;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
	
    // inPosition is float4 itself, seems like there is no need to extend it explicitly here
    output.outPosition = mul(float4(input.inPosition.xyz, 1.0f), gWorldViewProj);
    output.outTexCoord = input.inTexCoord;
    output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), gWorld)).xyz;
    // and here
    output.inWorldPos = mul(float4(input.inPosition.xyz, 1.0f), gWorld).xyz;
	  
    return output;
}