#include "EnemyMgr.h"
#include"../../FbxLoader.h"
#include"Constants.h"
#include"Enemy.h"

#include"../../DebugText.h"

#include"EnemyBody.h"

#include"QueenMgr.h"
#include"PanelMgr.h"

#include<algorithm>

EnemyMgr::EnemyMgr()
{
	QueenMgr::Instance();
	Enemy::LoadAudios();
	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		enemys.emplace_back(new Enemy());
		deadEnemy.push_front(enemys[i]);
	}
	model[PORN].reset((Model_HSM_ForIncrease*)FBXLoader::LoadFBXModel("resource/model/enemy/", "enemy.fbx"));
	model[KING].reset((Model_HSM_ForIncrease*)FBXLoader::LoadFBXModel("resource/model/king/", "enemy.fbx"));
	model[JACK].reset((Model_HSM_ForIncrease*)FBXLoader::LoadFBXModel("resource/model/jack/", "enemy.fbx"));
}

void EnemyMgr::Init()
{
	aliveEnemyNum = 0;
	for (auto& enemy : enemys)
	{
		enemy->Init();
	}
	EnemyBody::PrimitiveParticleMgr::Instance()->Init();
}

void EnemyMgr::Update()
{
	if (aliveEnemy.empty())return;

	//もう既に死んでいたらコンテナから消す
	aliveEnemy.remove_if(
		[&](std::weak_ptr<Enemy> x)
		{
			if (!x.lock()->GetIsAlive())
			{
				aliveEnemyNum--;
				deadEnemy.push_front(x);
			}
			return !x.lock()->GetIsAlive();
		});

	EnemyBody::PrimitiveParticleMgr::Instance()->Update();
}

void EnemyMgr::Draw()
{
	if (aliveEnemy.empty())return;

	ENEMY_TYPE nowType = ENEMY_TYPE_NUM;
	//各敵の一括描画
	for (auto enemy : aliveEnemy)
	{
		if (nowType != enemy.lock()->GetEnemyType())
		{
			//モデル情報のセット
			nowType = enemy.lock()->GetEnemyType();

			if (0 <= nowType && nowType < ENEMY_TYPE_NUM)
			model[nowType]->PreDraw();
		}
		if (0 <= nowType && nowType < ENEMY_TYPE_NUM)
		model[nowType]->DrawByComponent(enemy.lock()->GetComponent());
	}
	EnemyBody::PrimitiveParticleMgr::Instance()->Draw();

	//DebugText::FormatPrint(0,16 * 1.5f * 3,1.5f,"EnemyPanelNum")
}

void EnemyMgr::Finalize()
{
	while (!aliveEnemy.empty())
	{
		//エネミーを死亡させて死亡エネミーリストに移動
		deadEnemy.splice_after(deadEnemy.before_begin(), std::move(aliveEnemy), aliveEnemy.before_begin());
	}
}

void EnemyMgr::AllDead()
{
	for (auto& enemy : aliveEnemy)
	{
		enemy.lock()->Dead();
	}
}

void EnemyMgr::Generate(const EnemyGenerateHelper& InitData)
{
	if (deadEnemy.empty())return;

	if (InitData.type == QUEEN)
	{
		QueenMgr::Instance()->Generate(InitData);
	}
	else
	{
		//死亡エネミーリストから一体スポーン
		deadEnemy.begin()->lock()->Spawn(InitData);
		//スポーンしたエネミーを生存エネミーリストに移動
		aliveEnemy.splice_after(aliveEnemy.before_begin(), std::move(deadEnemy), deadEnemy.before_begin());

		aliveEnemyNum++;
	}
}

void EnemyMgr::RandGenerate(const int& LayerIndex)
{
	const int generateNum = MyFunc::GetRand(ADD_ENEMY_NUM_BY_ADD_LAYER_MIN, ADD_ENEMY_NUM_BY_ADD_LAYER_MAX);
	for (int enemyNum = 0; enemyNum < generateNum; ++enemyNum)
	{
		EnemyGenerateHelper info;
		do
		{
			info.type = (ENEMY_TYPE)MyFunc::GetRand(ENEMY_TYPE_NUM - 1);
		} while (info.type == QUEEN);
		info.isClockWise = MyFunc::GetRand(1) ? true : false;
		info.layerNum = LayerIndex;
		info.posAngle = MyFunc::GetRand(359) * (PI / 180.0f);
		this->Generate(info);
	}

	const int generateQueenNum = MyFunc::GetRand(ADD_QUEEN_NUM_BY_ADD_LAYER);
	for (int queenNum = 0; queenNum < generateQueenNum; ++queenNum)
	{
		EnemyGenerateHelper info;
		info.type = QUEEN;
		info.isClockWise = MyFunc::GetRand(1) ? true : false;
		info.layerNum = LayerIndex;
		info.posAngle = MyFunc::GetRand(359) * (PI / 180.0f);
		this->Generate(info);
	}
}

int EnemyMgr::ExistEnemy(const int& PanelIndex, const int& LayerNum)
{
	int num = 0;
	for (auto& enemy : aliveEnemy)
	{
		if (enemy.lock()->GetPanelIndex() == PanelIndex && enemy.lock()->Getlayer() <= LayerNum)
		{
			num++;
		}
	}
	return num;
}
