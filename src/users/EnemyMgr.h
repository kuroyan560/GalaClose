#pragma once
#include"../../Singleton.h"
#include"../../Model_HSM.h"
#include<memory>
#include"GameObjectInterFace.h"
#include<forward_list>
#include"EnemyGenerateHelper.h"

class Enemy;

class EnemyMgr : public Singleton<EnemyMgr>, GameObjectInterFace
{
	friend class Singleton<EnemyMgr>;
	EnemyMgr();

	//敵のモデル
	std::shared_ptr<Model_HSM_ForIncrease> model[ENEMY_TYPE_NUM];

	//敵のインスタンス配列
	std::vector<std::shared_ptr<Enemy>>enemys;

	//死亡中の敵リスト
	std::forward_list<std::weak_ptr<Enemy>>deadEnemy;
	//生存中の敵リスト
	std::forward_list<std::weak_ptr<Enemy>>aliveEnemy;
	int aliveEnemyNum = 0;

public:
	void Init()override;
	void Update()override;
	void Draw()override;	//敵を一括描画処理
	void Finalize()override;

	void AllDead();

	//敵の生成
	void Generate(const EnemyGenerateHelper& InitData);
	void RandGenerate(const int& LayerIndex);

	//指定のパネル内、指定の階層番号以下の階層に敵が存在するか
	int ExistEnemy(const int& PanelIndex, const int& LayerNum);

	const int& GetAliveEnemyNum() { return aliveEnemyNum; }
};