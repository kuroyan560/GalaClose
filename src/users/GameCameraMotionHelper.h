#pragma once
#include<DirectXMath.h>
using namespace DirectX;
#include"../../Easing.h"

struct GameCameraMotionHelper
{
	XMFLOAT3 endEye = { 0,0,0 };	//モーション終了時の視点座標
	XMFLOAT3 endTarget = { 0,0,0 };	//モーション終了時の注視点座標
	int totalTime = 0;	//モーション完了までにかかる時間
	EASE_CHANGE_TYPE easingChangeType = In;	//In,Out,InOut
	EASING_TYPE easingType = Exp;	//イージングの種類
	bool returnFlg = true;

	GameCameraMotionHelper() {};
	GameCameraMotionHelper(const GameCameraMotionHelper& rhs)
		:endEye(rhs.endEye), endTarget(rhs.endTarget), totalTime(rhs.totalTime), easingChangeType(rhs.easingChangeType), easingType(rhs.easingType),returnFlg(rhs.returnFlg) {};
};