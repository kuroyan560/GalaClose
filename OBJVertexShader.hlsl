#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(view, wpos); //�r���[�ϊ�
	
    output.depthInView = output.svpos.z; //�J�������猩��Z

    output.svpos = mul(proj, output.svpos); //�v���W�F�N�V�����ϊ�
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;
	output.ray = normalize(pos.xyz - cameraPos);

	return output;
}