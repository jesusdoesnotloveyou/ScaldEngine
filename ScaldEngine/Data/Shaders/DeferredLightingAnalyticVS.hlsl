// Common vertex shader for spot and point lights

#include "Common.hlsli"
#include "LightCommon.hlsli"

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
    // for instancing (not sure that's the best way to implement this logic)
    nointerpolation uint outInstanceID : InstanceID;
};

VS_OUT main(float4 inPosition : POSITION0, uint id : SV_VertexID)
{
    VS_OUT output = (VS_OUT) 0;
    
    output.outPosition = mul(inPosition, gPerObject.World);
    output.outPosition = mul(output.outPosition, gPerFrame.View);
    output.outPosition = mul(output.outPosition, gPerFrame.Proj);

    return output;
}