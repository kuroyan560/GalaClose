#pragma once
#include<DirectXMath.h>
using namespace DirectX;
#include"../../Easing.h"

struct GameCameraMotionHelper
{
	XMFLOAT3 endEye = { 0,0,0 };	//���[�V�����I�����̎��_���W
	XMFLOAT3 endTarget = { 0,0,0 };	//���[�V�����I�����̒����_���W
	int totalTime = 0;	//���[�V���������܂łɂ����鎞��
	EASE_CHANGE_TYPE easingChangeType = In;	//In,Out,InOut
	EASING_TYPE easingType = Exp;	//�C�[�W���O�̎��
	bool returnFlg = true;

	GameCameraMotionHelper() {};
	GameCameraMotionHelper(const GameCameraMotionHelper& rhs)
		:endEye(rhs.endEye), endTarget(rhs.endTarget), totalTime(rhs.totalTime), easingChangeType(rhs.easingChangeType), easingType(rhs.easingType),returnFlg(rhs.returnFlg) {};
};