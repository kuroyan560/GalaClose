#pragma once
#include"Enemy.h"
#include<memory>
class Model_HSM_ForIncrease;

class TitleEnemy : public Enemy
{
	std::shared_ptr<Model_HSM_ForIncrease>model;
	bool deadTrigger = false;
public:
	TitleEnemy(const std::string& Dir, const std::string& FileName);
	void Spawn(const EnemyGenerateHelper& InitData)override;
	void DeadTriggerReset(){ deadTrigger = false; }
	void Draw()override;
	void Dead()override;

	bool& DeadTrigger() { return deadTrigger; }
};