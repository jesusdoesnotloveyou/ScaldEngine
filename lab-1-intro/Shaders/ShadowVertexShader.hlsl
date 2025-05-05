
// MOVE TO CORE PIXEL SHADER //////////

Texture2D shaderTexture : register(t0);
//The depthMapTexture is the shadow map. This texture contains the scene depth buffer rendered from the light's perspective.
Texture2D depthMapTexture : register(t1);
///////////////////////////////////////

cbuffer cbPerObject : register(b0)
{
    matrix WVP;
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
    output.outPosition = mul(float4(input.inPosition), WVP);
    return output;
}