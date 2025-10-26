cbuffer cbPerObject : register(b0)
{
    matrix gWorld;
    matrix gView; // for simple shadow - lightView, for cascades - nothing
    matrix gProjection; // for simple shadow - lightProjection, for cascades - nothing
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