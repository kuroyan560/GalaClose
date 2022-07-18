#pragma once
#include<memory>
#include<DirectXMath.h>
using namespace DirectX;
class GameCameraMotionHelper;
class GameCamera;

class GameCameraMotion
{
	GameCamera* gameCam;
	int motionTimer = 0;	//モーション中の時間を測るタイマー
	std::shared_ptr<GameCameraMotionHelper>nowMotion;	//現在セットされているモーション
	XMFLOAT3 startEye;	//モーション開始時の視点座標
	XMFLOAT3 startTarget;	//モーション開始時の回転量

public:
	GameCameraMotion(GameCamera* GameCamera);
	void Init();
	void Update();
	void StartMotion(const GameCameraMotionHelper& MotionData);

	bool NowMotion() { return nowMotion != nullptr; }
};

