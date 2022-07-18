//�p�[�e�B�N���ꖇ �� ���_�P�� �W�I���g���V�F�[�_
//���̃p�[�e�B�N������
cbuffer cbuff0 : register(b0)
{
	matrix viewProjMat; // �r���[�v���W�F�N�V�����s��
	matrix billboardMat; // �r���{�[�h�s��

	/*
		�摜�𗐐��ŕς���ꍇ�A�P���̉摜�ɁA�`�悳��镡���̃p�[�e�B�N�����`����Ă���
		���̂Ƃ��̉摜�̕������i�p�[�e�B�N���̉摜�T�C�Y�͓����ɂ��Ȃ��ƌ��؂��j
	*/
	int imgSplitX;
	int imgSplitY;
};

struct VSOutput
{
	float4 pos : POSITION;	//���_���W
	float2 scale : SCALE;
	//min16uint2 �c unsigned short
	min16uint2 index : INDEX;	//�`�悵�����p�[�e�B�N���̈ʒu�ԍ��i�O�`�������j
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 uv : TEXCOORD;	//uv�l
	float4 color : COLOR;	//RGB �c �F���␳�AA �c �P���ȕs�����x
};