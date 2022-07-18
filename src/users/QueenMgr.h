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
	//インスタンス配列
	std::vector<std::shared_ptr<Queen>> queens;

	std::forward_list<std::weak_ptr<Queen>>deadQueen;

	std::forward_list<std::weak_ptr<Queen>> aliveQueen;

	int aliveQueenNum = 0;

public:
	QueenMgr();
	void Init()override;
	void Update()override;
	void Draw()override;	//敵を一括描画処理
	void Finalize()override;

	//Queenの生成
	void Generate(const EnemyGenerateHelper &InitData);

	const int &GetAliveQueenNum() { return aliveQueenNum; }

	void AllDead();

public://クイーン独自の関数
	//std::vector<SendData::QueenData> QueenDataSend();
	std::vector<std::weak_ptr<Queen>> QueenDataSend();
};
