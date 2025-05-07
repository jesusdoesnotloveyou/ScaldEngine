cbuffer cbPerObject : register(b0)
{
    matrix gWorldViewProj;
    matrix gWorld;
}

struct VS_IN
{
    float4 inPosition : POSITION0;
};

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.outPosition = mul(float4(input.inPosition), gWorldViewProj);
    return output;
}