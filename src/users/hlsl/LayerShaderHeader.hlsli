static const float ALPHA = 0.2f;
//static const float3 DEFAULT_COLOR = { (float) 0x53 / (float) 0xff, (float) 0x12 / (float) 0xff, (float) 0x90 / (float) 0xff };
//static const float3 ANOTHER_COLOR = { (float) 0x80 / (float) 0xff, (float) 0x27 / (float) 0xff, (float) 0xb9 / (float) 0xff };
static const float3 DEFAULT_COLOR = { 0.5f, 0.6f, 0.8f };
static const float3 ANOTHER_COLOR = { 0.1f, 0.2f, 0.4f };
static const float3 OUTLINE_COLOR = { 0.6f, 0.7f, 0.9f };

static const float LAYER_OFFSET_RADIUS = 8.0f; //���C���[���a�i���j

static const float LAYER_OUTLINE_OFFSET_THICKNESS = 0.3f;

cbuffer cbuff0 : register(b0)
{
    uint color;
    float pushedRate;
}

cbuffer cbuff1 : register(b1)
{
    matrix view; // �r���[�v���W�F�N�V�����s��
    matrix proj;
    float3 cameraPos; // �J�������W�i���[���h���W�j
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 pos : POSITION; // �V�X�e���p���_���W
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float3 ray : VECTOR; //�����x�N�g��
    float depthInView : CAM_Z; //�J������Ԃł�Z
};

struct PSOutput
{
    float4 color : SV_Target0; //�����_�[�^�[�Q�b�g�O�ɕ`������
    float depth : SV_Target1; //�����_�[�^�[�Q�b�g�P�ɕ`������
    float clip : SV_Target2;
};