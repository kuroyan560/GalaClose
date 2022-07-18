cbuffer cbuff0 : register(b0)
{
	matrix view; // �r���[�s��
	matrix proj;
	matrix world; // ���[���h�s��
	float3 cameraPos; // �J�������W�i���[���h���W�j
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0); // �A���r�G���g�W��
	float3 m_diffuse  : packoffset(c1); // �f�B�t���[�Y�W��
	float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
	float m_alpha : packoffset(c2.w);	// �A���t�@
}

// ���s�����̐�
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;    // ���C�g�ւ̕����̒P�ʃx�N�g��
	float3 lightcolor;    // ���C�g�̐F(RGB)
	uint actives;
};

//cbuffer cbuff2 : register(b2)
//{
//	float3 ambientColor;
//	DirLight dirLights[DIRLIGHT_NUM];
//}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal :NORMAL; // �@��
	float2 uv  :TEXCOORD; // uv�l
	float3 ray : VECTOR;	//�����x�N�g��
	
	//�J������Ԃł�Z
    float depthInView : CAM_Z;
};

struct PSOutput
{
    float4 color : SV_Target0; //�����_�[�^�[�Q�b�g�O�ɕ`������
    float depth : SV_Target1; //�����_�[�^�[�Q�b�g�P�ɕ`������
    float clip : SV_Target2;
};
