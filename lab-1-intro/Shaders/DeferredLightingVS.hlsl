cbuffer cbPerObject : register(b0)
{
    matrix gWorld;
    matrix gView;
    matrix gProjection;
};

struct UniLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float spot;
    float3 attenuation;
    float range;
    float3 position;
    float pad;
    int lightType;
    float3 pad2;
};

cbuffer LightCB : register(b1)
{
    UniLight Light;
}

struct VS_IN
{
    float4 inPosition : POSITION0;
    float2 inTexCoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD0;
};

VS_OUT main(VS_IN input, uint id : SV_VertexID)
{
    VS_OUT output = (VS_OUT) 0;
    
    const int DIRECTIONAL = 1;
    const int POINT = 2;
    const int SPOT = 3;
    
    [branch]
    if (Light.lightType == DIRECTIONAL)
    {
        output.outTexCoord = float2(id & 1, (id & 2) >> 1);
        output.outPosition = float4(output.outTexCoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    }
    else if (Light.lightType == POINT || Light.lightType == SPOT)
    {
        output.outPosition = mul(input.inPosition, gWorld);
        output.outPosition = mul(output.outPosition, gView);
        output.outPosition = mul(output.outPosition, gProjection);
    }

    return output;
}