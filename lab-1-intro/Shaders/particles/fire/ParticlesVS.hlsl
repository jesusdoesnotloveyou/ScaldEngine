struct VS_IN
{
    uint inVertexID : SV_VertexID;
};

struct SortListStruct
{
    uint index;
    float distanceSq;
};

StructuredBuffer<SortListStruct> sortListBuffer : register(t0);

struct VS_OUT
{
    uint outParticleIndex : INDEX;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.outParticleIndex = sortListBuffer[input.inVertexID].index;
    return output;
}