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

	//�G�̃��f��
	std::shared_ptr<Model_HSM_ForIncrease> model[ENEMY_TYPE_NUM];

	//�G�̃C���X�^���X�z��
	std::vector<std::shared_ptr<Enemy>>enemys;

	//���S���̓G���X�g
	std::forward_list<std::weak_ptr<Enemy>>deadEnemy;
	//�������̓G���X�g
	std::forward_list<std::weak_ptr<Enemy>>aliveEnemy;
	int aliveEnemyNum = 0;

public:
	void Init()override;
	void Update()override;
	void Draw()override;	//�G���ꊇ�`�揈��
	void Finalize()override;

	void AllDead();

	//�G�̐���
	void Generate(const EnemyGenerateHelper& InitData);
	void RandGenerate(const int& LayerIndex);

	//�w��̃p�l�����A�w��̊K�w�ԍ��ȉ��̊K�w�ɓG�����݂��邩
	int ExistEnemy(const int& PanelIndex, const int& LayerNum);

	const int& GetAliveEnemyNum() { return aliveEnemyNum; }
};