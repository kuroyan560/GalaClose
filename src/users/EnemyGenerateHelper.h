#pragma once
#include<memory>
#include"Constants.h"
class Panel;

struct EnemyGenerateHelper
{
	ENEMY_TYPE type = PORN;
	bool isClockWise = true;	//����
	float posAngle = 0.0f;	//���݈ʒu
	int layerNum = 0;
	float speedRate = 1.0f;
};