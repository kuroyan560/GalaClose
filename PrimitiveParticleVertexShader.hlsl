#include"PrimitiveParticleHeader.hlsli"
VSOutput main(float4 pos : POSITION, float3 rotate : ROTATE, float radius : RADIUS, float4 color : COLOR, min16uint primitive : PRIMITIVE)
{
    VSOutput output;
    output.primitive = primitive;
    output.pos = pos;
    output.rotate = rotate;
    output.radius = radius;
    output.color = color;
    return output;
}