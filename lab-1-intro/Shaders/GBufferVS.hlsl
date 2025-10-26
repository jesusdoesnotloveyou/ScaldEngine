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
    
    output.outTexCoord = float2(id & 1, (id & 2) >> 1);
    output.outPosition = float4(output.outTexCoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    
    return output;
}