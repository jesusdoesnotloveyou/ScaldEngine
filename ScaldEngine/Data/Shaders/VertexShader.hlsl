cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gInvTransposeWorld;
};

// forward rendering and simple shadows stuff
// won't work, because there is no such structure on the cpu side as well as code that bins it to the shader
cbuffer cbLightViewProj : register(b1)
{
    matrix gLightViewProj;
}

cbuffer cbPerFrame : register(b2)
{
    float4x4 gView;
    float4x4 gProjection;
    float4x4 gViewProj;
    float4 gEyePos;
}

struct VS_IN
{
    float4 inPosition : POSITION0;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
};

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD0;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POSITION;
    float4 outWorldView : POSITION;
    float4 outLightSpacePos : TEXCOORD1;
};


VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
	
    float4 modelPos = mul(float4(input.inPosition.xyz, 1.0f), gWorld);
    output.outWorldView = mul(modelPos, gView);
    output.outPosition = mul(output.outWorldView, gProjection);
    
    output.outTexCoord = input.inTexCoord;
    
    // has to be normalized in pixel shader
    output.outNormal = mul(input.inNormal, (float3x3) gInvTransposeWorld);
    
    output.outWorldPos = modelPos.xyz;

    float4 lightSpacePos = mul(modelPos, gLightViewProj);
    output.outLightSpacePos = lightSpacePos;

    return output;
}