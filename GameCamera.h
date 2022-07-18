#pragma once
#include"Camera.h"
#include"OBJModel.h"
#include"Singleton.h"

#include"WinApp.h"

#include"src/users/GameCameraMotion.h"
#include"src/users/GameCameraMotionHelper.h"

#include"Singleton.h"

class ActEditorsForCam;
class EventMgr;

class GameCamera : public Singleton<GameCamera>, public Camera
{
	friend class Singleton<GameCamera>;
	friend class Master;
	friend void ImguiCommand_Camera();
	friend class ActEditorsForCam;
	friend class GameCameraMotion;

	bool changedOffset = false;

	int shakeTimer = 0;

	int shakeSpan = 0;
	float shakeRandMax = 0;
	float shakeRandMin = 0;
	XMFLOAT3 shakeOffset = { 0.0f,0.0f,0.0f };

	GameCameraMotion motion;
	//void UpdateViewMat()override;

	GameCameraMotionHelper zoomMotion;
	GameCameraMotionHelper returnDefaultMotion;
	GameCameraMotionHelper zoomToEnemyMotion;
	GameCameraMotionHelper explosionMotion;

	int debug = 0;

	bool explosion = false;

	GameCamera();
public:
	virtual ~GameCamera();
	void Init()override;

	void Update()override;

	//void EnemyKillZoom(const int& PanelIndex);
	void ZoomToCenter();
	void ReturnToDefault();
	void ExplosionCam(const XMFLOAT3& DeadEnemyPos);
	void ExplosionCam();

	void CamShake();

	bool NowMotion() { return motion.NowMotion(); }
};

