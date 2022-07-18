#pragma once
#include"Scene.h"
#include"CenterPillar.h"
#include"GameCamera.h"
#include"Player.h"

#include"Object2D.h"

class GameScene : public Scene
{
	int bgmHandle;

	CenterPillar center;
	int nowCam = 1;

	Object2D* test = nullptr;

public:
	GameScene();
	~GameScene();
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;
};

