struct Particle
{
    float4 initialColor;
    float4 endColor;
    float3 position;
    float _1;
    float3 prevPosition;
    float _2;
    float3 velocity;
    float _3;
    float3 acceleration;
    float _4;
    float maxLifetime;
    float lifetime;
    float initialSize;
    float endSize;
    float initialWeight;
    float endWeight;
};

cbuffer CameraData : register(b0)
{
    matrix view;
    matrix proj;
    float3 camForward;
    float _1;
    float3 camUp;
    float _2;
};

struct GS_IN
{
    uint particleIndex : INDEX;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

StructuredBuffer<Particle> particlePool : register(t0);

[maxvertexcount(4)]
void main(point GS_IN input[1], inout TriangleStream<GS_OUT> triStream)
{
    Particle p = particlePool[input[0].particleIndex];

    float3 center = p.position;
    float size = lerp(p.initialSize, p.endSize, p.lifetime / p.maxLifetime);
    float4 color = lerp(p.initialColor, p.endColor, p.lifetime / p.maxLifetime);

    float3 camRight = normalize(cross(camForward, camUp));

    // float3 forward = -normalize(view._13_23_33); 
    float3 up = normalize(view._12_22_32) * 0.5f * size;
    float3 right = normalize(view._11_21_31) * 0.5f * size;

    float3 positions[4] =
    {
        center - right + up,
        center + right + up,
        center - right - up,
        center + right - up
    };

    float2 uvs[4] =
    {
        float2(0, 0),
        float2(1, 0),
        float2(0, 1),
        float2(1, 1)
    };

    GS_OUT output = (GS_OUT) 0;

    // Order: 0-1-2-3
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        output.pos = mul(float4(positions[i], 1.0f), view);
        output.pos /= output.pos.w;
        output.pos = mul(output.pos, proj);
        //output.pos /= output.pos.w;
        output.uv = uvs[i];
        output.color = color;
        triStream.Append(output);
    }
 
    triStream.RestartStrip();
}