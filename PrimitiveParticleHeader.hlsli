cbuffer cbuff0 : register(b0)
{
    matrix view; // �r���[�v���W�F�N�V�����s��
    matrix proj;
    float3 cameraPos; // �J�������W�i���[���h���W�j
}

struct VSOutput
{
    float4 pos : POSITION; //���_���W
    float3 rotate : ROTATE;
    float radius : RADIUS;
    float4 color : COLOR;
    min16uint primitive : PRIMITIVE; //�`��
	//min16uint �c unsigned short
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; // �@��
    float4 color : COLOR;
    float3 ray : VECTOR; //�����x�N�g��
};