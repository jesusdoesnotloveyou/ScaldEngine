cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gInvTransposeWorld;
}

struct VS_IN
{
    float4 inPosition : POSITION0;
};

struct VS_OUT
{
    float4 outPosition : POSITION;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.outPosition = mul(float4(input.inPosition), gWorld);
    return output;
}