#pragma once
#include"GameObjectInterFace.h"
#include<memory>
class Model_HSM_Normal;
#include"../../DirtyFlag.h"
#include"../../Singleton.h"

class Panel;

class Player : public GameObjectInterFace, public Singleton<Player>
{
	friend class Singleton<Player>;
	Player();

	static const int MUSICAL_SCALE_NUM = 8;
	int arrowJumpSeHandle[MUSICAL_SCALE_NUM];	//連続入力時のSE

	std::shared_ptr<Panel>selectPanel;

	//static const int LONG_INPUT_FRAME = 10;
	int inputWaitFrame = 0;
	int pushLayerNum = 0;
	std::shared_ptr<Model_HSM_Normal>arrowModel;

	int arrowPushTimer = 0;

	float pushRadiusOffset = 0.0f;
	float startRadius;
	float endRadius;

	float pushScaleOffset = 0.0f;
	float startScale;
	float endScale;

	float pushAngleOffset = 0.0f;
	float startAngle;
	float endAngle;

	DirtyFlag dirty;

	//直前に入力したボタン
	//int oldKeyCode;
	//int repeatTimer;
	//bool clockWise = false;

	int selectSe;

	void ArrowMove();
	void PushInputUpdate();

	bool MoveClockWise();
	bool MoveUnClockWise();

public:
	int selectPanelIndex = 0;

	void Init()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;

	const int& GetPushPower() { return pushLayerNum; }
};
