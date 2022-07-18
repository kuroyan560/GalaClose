#include "TitleEnemy.h"
#include"../../FbxLoader.h"

TitleEnemy::TitleEnemy(const std::string& Dir, const std::string& FileName)
{
	model.reset((Model_HSM_ForIncrease*)FBXLoader::LoadFBXModel(Dir, FileName));
}

void TitleEnemy::Spawn(const EnemyGenerateHelper& InitData)
{
	Enemy::Spawn(InitData);
	spawnTimer = 30;
	float scl = 1.07f;
	component.transform.scale = { scl,scl,scl };
	component.TransformMapping();
	enemyExplosion.Reset(type, false);	//”š’e‰ðœ
}

void TitleEnemy::Draw()
{
	if (isAlive)
	{
		model->PreDraw();
		model->DrawByComponent(component);
	}
}

void TitleEnemy::Dead()
{
	Enemy::Dead();
	deadTrigger = true;
}
