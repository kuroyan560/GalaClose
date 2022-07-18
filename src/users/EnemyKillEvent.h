#pragma once
#include<vector>
#include"../../Singleton.h"

class Enemy;

class EnemyKillEvent : public Singleton<EnemyKillEvent>
{
	EnemyKillEvent() {};
	friend class Singleton<EnemyKillEvent>;

	//€‚ÊƒGƒlƒ~[
	std::vector<Enemy*>killedEnemys;

	//“G€–SŠÔŠu‚ÌŠÔŒv‘ª
	int killTimer;

	//ŠK‘w‚Ì“G‘Sˆõ€–SŒã‚Ì‘Ò‹@ŠÔŒv‘ª
	int waitTimer;

	int killEnemyNum;

public:
	void Init();
	void Update();
	bool NowEvent();	//ƒLƒ‹‰‰o’†‚©
	void PushKilledEnemy(Enemy* Enemy) { killedEnemys.push_back(Enemy); }

	const int& GetKillEnemyNum() { return killEnemyNum; }
};