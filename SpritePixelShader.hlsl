#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);	//�O�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//�O�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(VSOutput input) :SV_TARGET
{
	float4 texColor =  tex.Sample(smp,input.uv) * color;
    
    PSOutput output;
    output.color = texColor;
    output.clip = 1;
    if (output.color.a == 0)
    {
        output.clip = 0;
    }
    
    return output;
}