cbuffer cbuff0 : register(b0)
{
	matrix view; // �r���[�s��
	matrix proj;
	matrix world; // ���[���h�s��
	float3 cameraPos; // �J�������W�i���[���h���W�j
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal :NORMAL; // �@��
	float2 uv  :TEXCOORD; // uv�l
};