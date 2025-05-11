cbuffer cbPerObject : register(b0)
{
    matrix gWorldViewProj;
    matrix gWorld;
};

cbuffer cbLightViewProj : register(b1)
{
    matrix gLightViewProj;
}

cbuffer CBufChangeOnResize : register(b2)
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
    float2 outTexCoord : TEXCOORD0;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POSITION;
    float4 outLightSpacePos : TEXCOORD1;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
	
    output.outPosition = mul(float4(input.inPosition.xyz, 1.0f), gWorldViewProj);
    output.outTexCoord = input.inTexCoord;
    output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), gWorld)).xyz;
    
    float4 modelPos = mul(float4(input.inPosition.xyz, 1.0f), gWorld);
    output.outWorldPos = modelPos.xyz;
	
    float4 lightSpacePos = mul(modelPos, gLightViewProj);
    output.outLightSpacePos = lightSpacePos;

    return output;
}