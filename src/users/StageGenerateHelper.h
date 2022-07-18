#pragma once
#include "EnemyGenerateHelper.h"
#include <vector>
struct StageGenerateHelper
{
	int panelNum = 4;
	int layerNum = 2;
	int color = 0;
	std::vector<EnemyGenerateHelper> enemysData;
};