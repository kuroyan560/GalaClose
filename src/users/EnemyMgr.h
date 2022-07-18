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

	//“G‚Ìƒ‚ƒfƒ‹
	std::shared_ptr<Model_HSM_ForIncrease> model[ENEMY_TYPE_NUM];

	//“G‚ÌƒCƒ“ƒXƒ^ƒ“ƒX”z—ñ
	std::vector<std::shared_ptr<Enemy>>enemys;

	//€–S’†‚Ì“GƒŠƒXƒg
	std::forward_list<std::weak_ptr<Enemy>>deadEnemy;
	//¶‘¶’†‚Ì“GƒŠƒXƒg
	std::forward_list<std::weak_ptr<Enemy>>aliveEnemy;
	int aliveEnemyNum = 0;

public:
	void Init()override;
	void Update()override;
	void Draw()override;	//“G‚ğˆêŠ‡•`‰æˆ—
	void Finalize()override;

	void AllDead();

	//“G‚Ì¶¬
	void Generate(const EnemyGenerateHelper& InitData);
	void RandGenerate(const int& LayerIndex);

	//w’è‚Ìƒpƒlƒ‹“àAw’è‚ÌŠK‘w”Ô†ˆÈ‰º‚ÌŠK‘w‚É“G‚ª‘¶İ‚·‚é‚©
	int ExistEnemy(const int& PanelIndex, const int& LayerNum);

	const int& GetAliveEnemyNum() { return aliveEnemyNum; }
};