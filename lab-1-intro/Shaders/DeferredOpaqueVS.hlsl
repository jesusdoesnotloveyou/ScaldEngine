cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gInvTransWorld;
    float4x4 gView;
    float4x4 gProjection;
};

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
    float3 outWorld : POSITION0;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.inPosition.w = 1.0f;

    float4 model = mul(input.inPosition, gWorld);
    output.outWorld = model.xyz;
    output.outPosition = mul(model, gView);
    output.outPosition = mul(output.outPosition, gProjection);
    
    output.outTexCoord = input.inTexCoord;
    
    output.outNormal = mul(input.inNormal, (float3x3) gInvTransWorld);
    output.outNormal = normalize(output.outNormal);

    return output;
}
