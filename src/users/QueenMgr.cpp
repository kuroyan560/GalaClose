#include "QueenMgr.h"
#include "Constants.h"
#include"../../FbxLoader.h"

QueenMgr::QueenMgr()
{
	for (int i = 0; i < QUEEN_MAX; i++)
	{
		queens.emplace_back(new Queen());
		deadQueen.push_front(queens[i]);
	}
	model.reset((Model_HSM_ForIncrease *)FBXLoader::LoadFBXModel("resource/model/queen/", "enemy.fbx"));
}

void QueenMgr::Init()
{
}

void QueenMgr::Update()
{
	if (aliveQueen.empty())return;

	//‚à‚¤‚·‚Å‚ÉŽ€‚ñ‚Å‚¢‚½‚çƒRƒ“ƒeƒi‚©‚çÁ‚·
	aliveQueen.remove_if([&](std::weak_ptr<Queen> x)
		{
			if (!x.lock()->GetIsAlive())
			{
				aliveQueenNum--;
				deadQueen.push_front(x);
			}
			return !x.lock()->GetIsAlive();
		}
	);
}

void QueenMgr::Draw()
{
	if (aliveQueen.empty())return;
	model->PreDraw();
	//Še“G‚ÌˆêŠ‡•`‰æ
	for (auto queen : aliveQueen)
	{
		model->DrawByComponent(queen.lock()->GetComponent());
	}
}

void QueenMgr::Finalize()
{
	while (!aliveQueen.empty())
	{
		deadQueen.splice_after(deadQueen.before_begin(), std::move(aliveQueen), aliveQueen.before_begin());
	}
}

void QueenMgr::Generate(const EnemyGenerateHelper &InitData)
{
	if (deadQueen.empty())return;

	deadQueen.begin()->lock()->Spawn(InitData);

	aliveQueen.splice_after(aliveQueen.before_begin(), std::move(deadQueen), deadQueen.before_begin());

	aliveQueenNum++;
}

void QueenMgr::AllDead()
{
	for (auto queen : aliveQueen)
	{
		queen.lock()->Dead();
	}
}

std::vector<std::weak_ptr<Queen>> QueenMgr::QueenDataSend()
{
	vector<std::weak_ptr<Queen>> queensData;

	std::weak_ptr<Queen> tmp;

	for (auto& queen : aliveQueen)
	{
		tmp = queen;
		queensData.emplace_back(tmp);
	}

	return queensData;
}
