#include "Common.hlsli"

// Shader doesn't work. Ensure that light sources' info are passed (from CPU) to constant buffers (GPU).
// PerObjectConstants (Mesh info) and PerFrameConstants (View, Proj, Camera info).
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
	
    float4 modelPos = mul(float4(input.inPosition.xyz, 1.0f), gPerObject.World);
    output.outWorldView = mul(modelPos, gPerFrame.View);
    output.outPosition = mul(output.outWorldView, gPerFrame.Proj);
    
    output.outTexCoord = input.inTexCoord;
    
    // has to be normalized in pixel shader
    output.outNormal = mul(input.inNormal, (float3x3) gPerObject.InvTransposeWorld);
    
    output.outWorldPos = modelPos.xyz;

    float4 lightSpacePos = mul(modelPos, gPerFrame.ViewProj);
    output.outLightSpacePos = lightSpacePos;

    return output;
}