#define TRANSPOSE_BLOCK_SIZE 16

cbuffer CB : register(b0)
{
    unsigned int g_iLevel;
    unsigned int g_iLevelMask;
    unsigned int g_iWidth;
    unsigned int g_iHeight;
};

StructuredBuffer<unsigned int> Input : register(t0);
RWStructuredBuffer<unsigned int> Data : register(u0);

//uint index = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * GROUP_COUNT_Y * THREAD_IN_GROUP_TOTAL + groupIndex;

groupshared unsigned int transpose_shared_data[TRANSPOSE_BLOCK_SIZE * TRANSPOSE_BLOCK_SIZE];

// MatrixTranspose
[numthreads(TRANSPOSE_BLOCK_SIZE, TRANSPOSE_BLOCK_SIZE, 1)]
void main(uint3 Gid : SV_GroupID,
                      uint3 DTid : SV_DispatchThreadID,
                      uint3 GTid : SV_GroupThreadID,
                      uint GI : SV_GroupIndex)
{
    transpose_shared_data[GI] = Input[DTid.y * g_iWidth + DTid.x];
    GroupMemoryBarrierWithGroupSync();
    uint2 XY = DTid.yx - GTid.yx + GTid.xy;
    Data[XY.y * g_iHeight + XY.x] = transpose_shared_data[GTid.x * TRANSPOSE_BLOCK_SIZE + GTid.y];
}