#pragma once
#include<memory>
#include"Constants.h"
class Panel;

struct EnemyGenerateHelper
{
	ENEMY_TYPE type = PORN;
	bool isClockWise = true;	//向き
	float posAngle = 0.0f;	//現在位置
	int layerNum = 0;
	float speedRate = 1.0f;
};