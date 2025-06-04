cbuffer ParticlesData : register(b0)
{
    float deltaTime;
    uint maxNumParticles;
    uint numEmitInThisFrame;
    uint numAliveParticles;
    float4 gEmitPos;
    float4 gEyePos;
};

struct Particle
{
    float4 pos;
    float4 prevPos;
    float4 velocity;
    float4 acceleration;
    float4 initialColor;
    float4 endColor;
    
    float maxLifeTime;
    float lifeTime;
    float initialSize;
    float endSize;
    float initialWeight;
    float endWeight;        
    float2 _pad;
};

struct Sort
{
    uint index;
    float distanceSq;
};

RWStructuredBuffer<Particle> ParticlePool : register(u0);   // Read-Write structured buffer, has to be created on CPU side UnorderedAccessView
RWStructuredBuffer<Sort> SortList : register(u1);           // Read-Write structured buffer, has to be created on CPU side UnorderedAccessView
AppendStructuredBuffer<uint> DeadList : register(u2);       // Read-Write structured buffer, has to be created on CPU side UnorderedAccessView

float distanceSquared(float3 a, float3 b)
{
    float3 d = a - b;
    return dot(d, d);
}

#define THREAD_GROUP_X 32
#define THREAD_GROUP_Y 32
#define THREAD_GROUP_TOTAL 1024

[numthreads(THREAD_GROUP_X, THREAD_GROUP_Y, 1)]
void main(uint3 DTid : SV_GroupThreadID, uint3 Gid : SV_GroupID)
{
    uint threadGroupOffset = THREAD_GROUP_TOTAL * (Gid.x + Gid.y * 32);
    uint sortStructIndex = threadGroupOffset + DTid.y * 32 + DTid.x;

    if (sortStructIndex >= numAliveParticles || sortStructIndex >= maxNumParticles) return;

    Sort sls = SortList[sortStructIndex];
    uint particleIndex = sls.index;
    Particle p = ParticlePool[particleIndex];

    p.lifeTime += deltaTime;
    p.prevPos = p.pos;
    p.pos += p.velocity * deltaTime;
    p.velocity += p.acceleration * deltaTime;

    [branch]
    if (p.lifeTime >= p.maxLifeTime)
    {
        DeadList.Append(particleIndex);
        sls.distanceSq = 100000.0f;
        SortList.DecrementCounter();
    }
    else
    {
        sls.distanceSq = distanceSquared(p.pos.xyz, gEyePos.xyz);
    }
    
    ParticlePool[particleIndex] = p;
    SortList[sortStructIndex] = sls;
}