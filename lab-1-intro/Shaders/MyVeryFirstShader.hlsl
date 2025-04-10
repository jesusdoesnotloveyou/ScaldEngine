cbuffer cbPerObject : register(b0)
{
    matrix gWorldViewProj;
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
};


VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
	
    output.outPosition = mul(float4(input.inPosition.xyz, 1.0f), gWorldViewProj);
    output.outTexCoord = input.inTexCoord;
    output.outNormal = input.inNormal;
	
	return output;
}