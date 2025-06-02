struct Particle
{
    float3 Position;
    float3 Velocity;
};

cbuffer Handler : register(b0)
{
    int GroupDim;
    uint MaxParticles;
    float DeltaTime;
};

RWStructuredBuffer<Particle> Particles : register(u0); // Read-Write structured buffer, has to be created on CPU side UnorderedAccessView

#define THREAD_GROUP_X 32
#define THREAD_GROUP_Y 32
#define THREAD_GROUP_TOTAL 1024

[numthreads(THREAD_GROUP_X, THREAD_GROUP_Y, 1)]
void main(uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = groupID.x * THREAD_GROUP_TOTAL + groupID.y * GroupDim * THREAD_GROUP_TOTAL + groupIndex;
	
	[flatten]
    if (index >= MaxParticles)
        return;

    Particle particle = Particles[index];

    float3 position = particle.Position;
    float3 velocity = particle.Velocity;

    // payload

    particle.Position = position + velocity * DeltaTime;
    particle.Velocity = velocity;

    Particles[index] = particle;
}