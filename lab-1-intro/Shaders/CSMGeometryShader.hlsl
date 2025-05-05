struct GS_IN
{
    float4 inPosition : POSITION;
};

struct GS_OUT
{
    float4 outPosition : SV_POSITION;
    uint arrInd : SV_RenderTargetArrayIndex;
};

struct CascadeData
{
    matrix ViewProj[4];
    float4 Distances;
};

cbuffer CascBuf : register(b0)
{
    CascadeData CascData;
}

[instance(4)]
[maxvertexcount(3)]
void main(triangle GS_IN p[3], in uint id : SV_GSInstanceID, inout TriangleStream<GS_OUT> stream)
{
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        GS_OUT gs = (GS_OUT) 0;
        gs.outPosition = mul(float4(p[i].inPosition.xyz, 1.0f), CascData.ViewProj[id]);

        gs.arrInd = id;
        stream.Append(gs);
    }
}