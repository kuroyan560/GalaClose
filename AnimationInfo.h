#pragma once
#include<vector>
#include<string>
using namespace std;
#include"MyFunc.h"
namespace AnimationInfo
{
	static const enum INTERPOLATION	//���
	{
		UNKNOWN_INTERPOLATION, CONSTANT, LINEAR, CUBIC, INTERPOLATION_NUM
	};
	static const enum CONSTANT_MODE
	{
		UNKNOWN_CONSTANT_MODE, STANDARD, NEXT
	};
	static const enum CUBIC_MODE
	{
		UNKNOWN_CUBIC_MODE, AUTO, AUTO_BREAK, TCB, USER, BREAK, USER_BREAK
	};
	static const enum TANGENT_WV_MODE
	{
		UNKNOWN_TANGENT_WV_MODE, NONE, RIGHT, NEXT_LEFT
	};

	struct Key
	{
		XMMATRIX value;
		int frame;
		INTERPOLATION interpolation = UNKNOWN_INTERPOLATION;
		CONSTANT_MODE constantMode = UNKNOWN_CONSTANT_MODE;
		CUBIC_MODE cubicMode = UNKNOWN_CUBIC_MODE;
		TANGENT_WV_MODE tangentWVMode_weight = UNKNOWN_TANGENT_WV_MODE;
		TANGENT_WV_MODE tangentWVMode_velocity = UNKNOWN_TANGENT_WV_MODE;
	};

	//static enum VALUE_MODE
	//{ 
	//	POS, ROTATE, SCALE, VALUE_MODE_NUM
	//};
	//static enum INDEX
	//{
	//	X, Y, Z, INDEX_NUM
	//};
	struct Animation	//�{�[���P��
	{
		std::string boneName;	//�{�[���̖��O
		std::vector<Key>keys;
	};

	//��������Z��
	//LINK �c �A�j���[�V�����O�̍Ō�̎p����animation�n�߂̎p���ɂ���
	static const enum AFFECT_MODE
	{
		ASSIGN, ADDITIONAL
	};
};