#include "2DShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1));	//�E�����@�����̃��C�g
	//float diffuse = saturate(dot(-light, input.normal));	//diffuse��[0,1]�͈̔͂�Clamp����
	//float brightness = diffuse + 0.3f;	//������ 0.3 �Ƃ��Čv�Z

	float4 texcolor = float4(tex.Sample(smp, input.uv));

	return texcolor;
	//return float4(texcolor.r * brightness, texcolor.g * brightness, texcolor.b * brightness, 1);
}