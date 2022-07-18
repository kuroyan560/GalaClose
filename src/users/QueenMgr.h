#pragma once
#include "../../Singleton.h"
#include "GameObjectInterFace.h"
#include "Queen.h"
#include <memory>
#include <forward_list>
#include "EnemyGenerateHelper.h"
#include"../../Model_HSM.h"

//namespace SendData
//{
//	struct QueenData
//	{
//		std::vector<movablePanel> panels;
//		float posAngle;
//	};
//};

class QueenMgr : public Singleton<QueenMgr>, GameObjectInterFace
{
	std::shared_ptr<Model_HSM_ForIncrease> model;
	//�C���X�^���X�z��
	std::vector<std::shared_ptr<Queen>> queens;

	std::forward_list<std::weak_ptr<Queen>>deadQueen;

	std::forward_list<std::weak_ptr<Queen>> aliveQueen;

	int aliveQueenNum = 0;

public:
	QueenMgr();
	void Init()override;
	void Update()override;
	void Draw()override;	//�G���ꊇ�`�揈��
	void Finalize()override;

	//Queen�̐���
	void Generate(const EnemyGenerateHelper &InitData);

	const int &GetAliveQueenNum() { return aliveQueenNum; }

	void AllDead();

public://�N�C�[���Ǝ��̊֐�
	//std::vector<SendData::QueenData> QueenDataSend();
	std::vector<std::weak_ptr<Queen>> QueenDataSend();
};
