#pragma once
#include"../../Singleton.h"
#include"GameObjectInterFace.h"
#include<memory>
#include<vector>
#include"../../Model_HSM.h"

class BackGround : public Singleton<BackGround>
{
	friend class Singleton<BackGround>;
	BackGround();
public:
	static const int CROSS_NUM = 4;
private:
	int emitWaveTimer = 0;
	int waveTimer;

	std::shared_ptr<Model_HSM_ForIncrease>crossModel;
	Model_HSM_Component crosses[CROSS_NUM][CROSS_NUM][CROSS_NUM];
	int easeTimer[CROSS_NUM];

public:
	void Init();
	void Update();
	void Draw();
	void Finalize() {};

	void Wave();
};

