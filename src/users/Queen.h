#pragma once
#include "Enemy.h"
#include <vector>

#include<memory>
class Object2D_Divide;

struct movablePanel
{
	int layerNum = 0;
	int panelNum = 0;
	bool ClockWise = false;
	bool CounterClockWise = false;
};
class Queen : public Enemy
{
public:
	Queen();
	void UniqueInit()override;
	void UniqueUpdate()override;
	void UniqueFinalize()override;
	void Draw()override;
	void Spawn(const EnemyGenerateHelper& InitData)override;
	void Dead()override;

	void PanelCheck();
	const vector<movablePanel> &GetmovablePanels();

	void AddDrainEnemy();

private:
	DirtyFlag drainDirty;
	int drainEnemyNum = 0;
	int drainTimer = 0;
	vector<movablePanel>movablePanels;

	//ãzé˚ÇµÇΩìGÇÃêî
	static const int DRAIN_ENEMY_NUM_MAX_DIGIT = 2;
	std::shared_ptr<Object2D_Divide>numObj2d[DRAIN_ENEMY_NUM_MAX_DIGIT];
};