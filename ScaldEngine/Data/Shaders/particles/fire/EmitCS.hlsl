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

cbuffer ParticlesData : register(b0)
{
    float deltaTime;
    uint maxNumParticles;
    uint numEmitInThisFrame;
    uint numAliveParticles;
    float4 gEmitPos;
    float4 gEyePos;
};

struct Sort
{
    uint index;
    float distanceSq;
};

float distanceSquared(float3 a, float3 b)
{
    float3 d = a - b;
    return dot(d, d);
}

StructuredBuffer<Particle> InjectionBuffer : register(t0);
RWStructuredBuffer<Particle> ParticlePool : register(u0);
RWStructuredBuffer<Sort> SortList : register(u1);
ConsumeStructuredBuffer<uint> DeadListToInit : register(u2);

#define THREAD_GROUP_X 32
#define THREAD_GROUP_Y 32
#define THREAD_GROUP_TOTAL 1024

[numthreads(THREAD_GROUP_X, THREAD_GROUP_Y, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint index = DTid.y * THREAD_GROUP_Y + DTid.x;
    uint numDeadParticles = maxNumParticles - numAliveParticles;
    
    if (index >= numDeadParticles || index >= numEmitInThisFrame || index >= maxNumParticles) return;
    
    uint particleIndex = DeadListToInit.Consume();
    
    Particle p = InjectionBuffer[index];
    
    ParticlePool[particleIndex] = p;
    
    Sort sls;
    sls.index = particleIndex;
    sls.distanceSq = distanceSquared(p.pos.xyz, gEyePos.xyz);
    
    uint sortIndex = SortList.IncrementCounter();
    SortList[sortIndex] = sls;
}