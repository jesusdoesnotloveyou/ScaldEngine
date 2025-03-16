cbuffer CBuf : register(b0)
{
    // matrices are column_major by default
    /*row_major */matrix transform;
};

cbuffer CBufChangeOnResize : register(b1)
{
    matrix mProjection;
}

struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

PS_IN main( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    output.pos = mul(input.pos, transform);
	output.col = input.col;
	
	return output;
}

//float4 PSMain( PS_IN input ) : SV_Target
//{
//	float4 col = input.col;
////#ifdef TEST
////	if (input.pos.x > 400) col = TCOLOR;
////#endif
//	return col;
//}