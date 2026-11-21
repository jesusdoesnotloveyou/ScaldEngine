#include "CommonTypes.hlsli"

#define PI 3.141592653589793

cbuffer cbPerObject : register(b0)
{
    PerObjectConstants gPerObject;
};

cbuffer cbPerFrame : register(b1)
{
    PerFrameConstants gPerFrame;
}