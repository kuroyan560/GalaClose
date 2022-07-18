#include"PrimitiveParticleHeader.hlsli"
SamplerState smp : register(s0); //０番スロットに設定されたサンプラー

float4 main(GSOutput input) : SV_TARGET
{
    //光の向かうベクトル（平行光線）
	//float3 light = normalize(float3(0,0,1));
    float3 light = normalize(float3(1, -1, 1));
	//ライトのカラー（白）
    float3 lightColor = float3(1, 1, 1);
    float4 result = input.color;
    
    float p = dot(input.normal * -1.0f, light.xyz);
    p = p * 0.5f + 0.5f;
    p = p * p;
    
    float3 toon;
    if(p < 0.2f)
    {
        toon = (0.85f, 0.85f, 0.85f);
    }
    else if(p < 0.5f)
    {
        toon = (1.0f, 1.0f, 1.0f);
    }
    else
    {
        toon = (1.15f, 1.15f, 1.15f);
    }
    
    result.xyz *= toon.xyz;
    return result;
}