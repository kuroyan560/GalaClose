#include"ParticleShaderHeader.hlsli"
Texture2D<float4> tex : register(t0);	//�O�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//�O�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(GSOutput input) : SV_TARGET
{
	return tex.Sample(smp,input.uv) * input.color;
}