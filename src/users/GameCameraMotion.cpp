#include "GameCameraMotion.h"
#include"GameCameraMotionHelper.h"
#include"../../GameCamera.h"
#include"../../Easing.h"

GameCameraMotion::GameCameraMotion(GameCamera* GameCamera)
	:gameCam(GameCamera)
{
}

void GameCameraMotion::Init()
{
	motionTimer = 0;
	nowMotion = nullptr;
}

void GameCameraMotion::Update()
{
	if (nowMotion != nullptr)
	{
		if (motionTimer)
		{
			int time = nowMotion->totalTime - motionTimer;
			gameCam->cameraInfo.eye = Easing::Ease(nowMotion->easingChangeType, nowMotion->easingType,
				time, nowMotion->totalTime, startEye, nowMotion->endEye);
			gameCam->target = Easing::Ease(nowMotion->easingChangeType, nowMotion->easingType,
				time, nowMotion->totalTime, startTarget, nowMotion->endTarget);
			motionTimer--;

			if (motionTimer == 0 && nowMotion->returnFlg)
			{
				nowMotion.reset();
				nowMotion = nullptr;
			}
		}
	}
}

void GameCameraMotion::StartMotion(const GameCameraMotionHelper& MotionData)
{
	nowMotion.reset(new GameCameraMotionHelper(MotionData));
	motionTimer = MotionData.totalTime;
	startEye = gameCam->cameraInfo.eye;
	startTarget = gameCam->target;
}
