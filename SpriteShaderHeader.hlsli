//�X�v���C�g�p

cbuffer cbuff0 : register(b0)
{
	float4 color;	//�F(RGBA)
	matrix mat;		//�R�c�ϊ��s��
};

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 uv : TEXCOORD;	//uv�l
};

struct PSOutput
{
    float4 color : SV_Target0; //�����_�[�^�[�Q�b�g�O�ɕ`������
    float depth : SV_Target1; //�����_�[�^�[�Q�b�g�P�ɕ`������
    float clip : SV_Target2;
};