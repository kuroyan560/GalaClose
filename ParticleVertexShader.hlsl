#include"ParticleShaderHeader.hlsli"
VSOutput main(float4 pos : POSITION, float2 scale : SCALE, min16uint2 index : INDEX, float4 color : COLOR)
{
	VSOutput output;
	output.pos = pos;
	output.scale = scale;
	output.index = index;
	output.color = color;
	return output;
}