struct Gbuffer
{
    float4 DiffuseSpec : SV_Target0;
    float3 WorldPos : SV_Target1;
    float3 Normal : SV_Target2;
};

struct PS_IN
{
    
};

[earlydepthstencil]
Gbuffer main(PS_IN input) : SV_Target
{
    Gbuffer output = (Gbuffer) 0;
    
    //output.LightAccumulation = (ambient + emissive);
    //output.Diffuse = diffuse;
    //output.Specular = float4(specular.rgb, log2(specularPower) / 10.5f);
    //output.NormalVS = N;
 
    return output;
}