#pragma once
#include"MyFunc.h"

struct BoneNode
{
	int boneIdx;	//�{�[���C���f�b�N�X
	MyDef::float3 startPos;	//�{�[����_(��]�̒��S)
	MyDef::float3 endPos;	//�{�[����[�_(���ۂ̃X�L�j���O�ɂ͗��p���Ȃ�)
	std::vector<BoneNode*>children;	//�q�m�[�h
};