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
    row_major matrix View[4];
    row_major matrix Proj[4];
    float4 Distances; // not used, so not filled on the CPU side
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
        gs.outPosition = mul(float4(p[i].inPosition.xyz, 1.0f), CascData.View[id]);
        float3 normView = normalize(gs.outPosition.xyz);
        
        gs.outPosition = mul(float4(gs.outPosition.xyz + normView * 0.00005f, 1.0f), CascData.Proj[id]);

        gs.arrInd = id;
        stream.Append(gs);
    }
}