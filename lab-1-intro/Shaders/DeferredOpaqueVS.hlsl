cbuffer cbPerObject
{
    matrix gWorld;
    matrix gView;
    matrix gProjection;
};

struct VS_IN
{
    float4 inPosition : POSITION;
    float2 inTexCoord : TEXCOORD0;
    float3 inNormal : NORMAL;
};

struct VS_OUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD0;
    float3 outNormal : NORMAL;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.inPosition.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.outPosition = mul(input.inPosition, gWorld);
    output.outPosition = mul(output.outPosition, gView);
    output.outPosition = mul(output.outPosition, gProjection);
    
    // Store the texture coordinates for the pixel shader.
    output.outTexCoord = input.inTexCoord;
    
    // Calculate the normal vector against the world matrix only.
    output.outNormal = mul(input.inNormal, (float3x3) gWorld);
	
    // Normalize the normal vector.
    output.outNormal = normalize(output.outNormal);

    return output;
}
