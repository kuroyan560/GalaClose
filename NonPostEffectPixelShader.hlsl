#include"PostEffectShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);	//０番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//０番スロットに設定されたサンプラー

//レンダーターゲットをスプライトとして描画するためだけのシェーダー（加工しない）
float4 main(VSOutput input) : SV_TARGET
{
	return tex.Sample(smp,input.uv);
}