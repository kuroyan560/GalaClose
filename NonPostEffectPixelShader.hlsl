#include"PostEffectShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);	//�O�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//�O�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//�����_�[�^�[�Q�b�g���X�v���C�g�Ƃ��ĕ`�悷�邽�߂����̃V�F�[�_�[�i���H���Ȃ��j
float4 main(VSOutput input) : SV_TARGET
{
	return tex.Sample(smp,input.uv);
}