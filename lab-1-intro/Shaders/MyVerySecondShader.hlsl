//struct PS_IN
//{
//    float4 pos : SV_POSITION;
//    float4 col : COLOR;
//};

float4 main(float4 pos : SV_POSITION, float4 col : COLOR) : SV_Target
{
    float4 color = col;
//#ifdef TEST
//	if (input.pos.x > 400) col = TCOLOR;
//#endif
    return color;
}