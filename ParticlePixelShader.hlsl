#include"ParticleShaderHeader.hlsli"
Texture2D<float4> tex : register(t0);	//０番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//０番スロットに設定されたサンプラー

float4 main(GSOutput input) : SV_TARGET
{
	return tex.Sample(smp,input.uv) * input.color;
}