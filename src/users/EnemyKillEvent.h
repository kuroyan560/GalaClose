#pragma once
#include<vector>
#include"../../Singleton.h"

class Enemy;

class EnemyKillEvent : public Singleton<EnemyKillEvent>
{
	EnemyKillEvent() {};
	friend class Singleton<EnemyKillEvent>;

	//���ʃG�l�~�[
	std::vector<Enemy*>killedEnemys;

	//�G���S�Ԋu�̎��Ԍv��
	int killTimer;

	//�K�w�̓G�S�����S��̑ҋ@���Ԍv��
	int waitTimer;

	int killEnemyNum;

public:
	void Init();
	void Update();
	bool NowEvent();	//�L�����o����
	void PushKilledEnemy(Enemy* Enemy) { killedEnemys.push_back(Enemy); }

	const int& GetKillEnemyNum() { return killEnemyNum; }
};