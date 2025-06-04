#define BITONIC_BLOCK_SIZE 512

cbuffer CB : register(b0)
{
    unsigned int g_iLevel;
    unsigned int g_iLevelMask;
    unsigned int g_iWidth;
    unsigned int g_iHeight;
};

struct ParticleDepth
{
    uint Index;
    float Depth;
};

RWStructuredBuffer<ParticleDepth> Data : register(u0);

groupshared ParticleDepth shared_data[BITONIC_BLOCK_SIZE];

// BitonicSort
[numthreads(BITONIC_BLOCK_SIZE, 1, 1)]
void main(uint3 Gid : SV_GroupID,
                  uint3 DTid : SV_DispatchThreadID,
                  uint3 GTid : SV_GroupThreadID,
                  uint GI : SV_GroupIndex)
{
    // Load shared data
    shared_data[GI] = Data[DTid.x];
    GroupMemoryBarrierWithGroupSync();
    
    // Sort the shared data
    for (unsigned int j = g_iLevel >> 1; j > 0; j >>= 1)
    {
        uint ixj = GI ^ j;
        if (ixj > GI)
        {
            ParticleDepth a = shared_data[GI];
            ParticleDepth b = shared_data[ixj];

            bool ascending = ((DTid.x & g_iLevelMask) == 0);
            if ((a.Depth > b.Depth) == ascending)
            {
                shared_data[GI] = b;
                shared_data[ixj] = a;
            }
        }
        GroupMemoryBarrierWithGroupSync();
    }
    // Store shared data
    Data[DTid.x] = shared_data[GI];
}