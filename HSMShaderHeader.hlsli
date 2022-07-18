cbuffer cbuff0 : register(b0)
{
	float4 masterColor;	//RGBA(0~1)
	bool shadow;
    float flash;
}

cbuffer cbuff1 : register(b1)
{
	float4 diffuse;
	float3 specular;
	float specular_c;
	float3 ambient;
	float alpha;
}

cbuffer cbuff2 : register(b2)
{
	matrix view; // �r���[�v���W�F�N�V�����s��
	matrix proj;
	float3 cameraPos; // �J�������W�i���[���h���W�j
}

cbuffer cbuff3 : register(b3)
{
	matrix world; // ���[���h�s��
}

cbuffer cbuff4 : register(b4)
{
    matrix bones[256];	//�{�[���s��
}

//cbuffer cbuff4 : register(b4)
//{
//	float3 dirDirection;
//	float3 dirColor;
//	float3 ptPosition;
//	float3 ptColor;
//	float ptRange;
//	float3 ambientLight;
//}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float3 worldpos : POSITION;
	float3 normal :NORMAL0; // �@��
	float4 vnormal : NORMAL1;	//�r���[�ϊ���̖@���x�N�g��
	float2 uv  :TEXCOORD; // uv�l
	float3 ray : VECTOR;	//�����x�N�g��

	//�J������Ԃł�Z
	float depthInView : CAM_Z;
};

struct PSOutput
{
	float4 color : SV_Target0;	//�����_�[�^�[�Q�b�g�O�ɕ`������
	float depth : SV_Target1;	//�����_�[�^�[�Q�b�g�P�ɕ`������
    float clip : SV_Target2;
};