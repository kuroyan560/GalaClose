#include "EnemyGenerate.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include"EnemyMgr.h"
#include"PanelMgr.h"

using namespace std;
using namespace MyFunc;


EnemyGenerate::EnemyGenerate()
{
	//敵の種類割合                    {PORN, QUEEN, KING, JACK}
	generatePattern.enemyRate = { 50, 2, 30, 10 };
	//敵の出現パターン                  {RANDOM, CIRCLE, WIDTH, PILLAR}
	generatePattern.patternRate = { 20, 30, 50, 20 };
	//敵の出現時間
	generatePattern.maxTimer = 120;
	//敵の出現数
	generatePattern.count = 5;
}

void EnemyGenerate::scoreModeGenerate()
{
	timer--;
	scoreModeGeneratepattern s = generatePattern;
	if (timer <= 0)
	{
		timer = s.maxTimer;
	}
	else
	{
		return;
	}

	vector<int> p = { s.patternRate.randumRate,s.patternRate.circleRate,s.patternRate.widthRate,s.patternRate.pillarRate };
	int PMax = 0;
	PMax--;

	for (int i = 0; i < p.size(); i++)
	{
		PMax += p[i];
	}
	int PNum = GetRand(PMax);
	int pat = 0;
	for (int i = 0; i < p.size(); i++)
	{
		if (PNum < p[i])
		{
			pat = i;
			break;
		}
		else
		{
			PNum -= p[i];
		}
	}
	switch (pat)//場合
	{
	case RANDUM:
		SpawnPattern::Randum(s.count, s.enemyRate);
		break;
	case CIRCLE:
		SpawnPattern::Ring(s.count, GetRand(PanelMgr::Instance()->GetMaxLayerNum() - 1), s.enemyRate);
		break;
	case WIDTH:
		SpawnPattern::Width(s.count, GetRand(PanelMgr::Instance()->GetPanelNum() - 1), GetRand(PanelMgr::Instance()->GetMaxLayerNum() - 1), s.enemyRate);
		break;
	case PILLAR:
		SpawnPattern::pillar(s.count, GetRand(PanelMgr::Instance()->GetPanelNum() - 1), s.enemyRate);
		break;
	default:
		break;
	}
}

void EnemyGenerate::SetGeneratePattern(scoreModeGeneratepattern info)
{
	generatePattern = info;
}

//void EnemyGenerate::Init()
//{
//	PanelMgr::Instance()->Init(stageData.panelNum, stageData.layerNum, stageData.color);
//	for (auto& enemy : stageData.enemysData)
//	{
//		EnemyMgr::Instance()->Generate(enemy);
//	}
//}

void EnemyGenerate::PanelsInit()
{
	PanelMgr::Instance()->Init(stageData.panelNum, stageData.layerNum, stageData.color);
}

void EnemyGenerate::EnemysInit()
{
	for (auto &enemy : stageData.enemysData)
	{
		EnemyMgr::Instance()->Generate(enemy);
	}
}

void EnemyGenerate::Update()
{
}

void EnemyGenerate::Finalize()
{
}

void EnemyGenerate::Draw()
{
}

void EnemyGenerate::LoadFile(const char *fileName)
{
	fstream file;

	file.open(fileName);

	if (file.fail())
	{
		return;
	}

	string line;
	std::vector<EnemyGenerateHelper>tmpEnemysData;

	while (getline(file, line))
	{
		istringstream lineS(line);
		string key;
		getline(lineS, key, ' ');

		if (key == "stage")
		{
			//パネルの枚数(横の個数)
			int count;
			lineS >> count;

			//奇数または (4<=n<=8) の範囲を超えていたら
			if ((count % 2 != 0) || (count < 4 && count > 8))
			{
				count = 4;
			}
			stageData.panelNum = count;

			lineS.seekg(1, ios_base::cur);
			//レイヤー数(縦の数)
			lineS >> stageData.layerNum;
			lineS.seekg(1, ios_base::cur);
			//しましまon/off
			lineS >> stageData.color;
		}


		if (key == "enemy")
		{
			EnemyGenerateHelper tmp;

			//タイプ
			int type;
			lineS >> type;
			tmp.type = (ENEMY_TYPE)type;
			lineS.seekg(1, ios_base::cur);

			//int panelNum;
			//向き
			lineS >> tmp.isClockWise;
			lineS.seekg(1, ios_base::cur);

			//現在のレイヤー
			lineS >> tmp.layerNum;
			lineS.seekg(1, ios_base::cur);

			//現在の位置
			float tmpPosA;

			//現在の位置
			lineS >> tmpPosA;
			while (tmpPosA >= 360.0f)
			{
				tmpPosA -= 360.0f;
			}
			float PI = 3.14159265;
			tmpPosA *= PI / 180;
			tmp.posAngle = tmpPosA;
			//lineS >> tmp.posAngle;
			lineS.seekg(1, ios_base::cur);

			lineS >> tmp.speedRate;

			tmpEnemysData.emplace_back(tmp);
		}
	}

	stageData.enemysData = tmpEnemysData;
	file.close();

	load = true;
}

const std::vector<EnemyGenerateHelper> &EnemyGenerate::EnemyOutput()
{
	return stageData.enemysData;
}

const StageGenerateHelper &EnemyGenerate::StageOutput()
{
	return stageData;
}

void SpawnPattern::Randum(int Count, EnemyRate type)
{
	EnemyGenerateHelper info;

	int maxLayerSize = PanelMgr::Instance()->GetMaxLayerNum();
	//通常の敵の生成
	for (int i = 0; i < Count; i++)
	{
		//do
		//{
		//	info.type = (ENEMY_TYPE)MyFunc::GetRand(ENEMY_TYPE_NUM - 1);
		//} while (info.type == QUEEN);
		info.type = TypeEnter(type);
		info.isClockWise = MyFunc::GetRand(1) ? true : false;
		//info.layerNum = LayerNum;
		info.posAngle = MyFunc::GetRand(359) * (PI / 180.0f);

		//パネルのサイズからランダムに生成
		auto tmpPanel = PanelMgr::Instance()->GetPanelByAngle(info.posAngle, true);
		int pushedLayer = tmpPanel.get()->pushedLayer;
		int nowLayerSize = maxLayerSize - pushedLayer;

		int layerNum = GetRand(nowLayerSize - 1);

		info.layerNum = layerNum;
		//パネル番号に合わせてパネルの生成番号に合わせて
		safeSpawn(info);
	}

}

void SpawnPattern::Ring(int Count, int LayerNum, EnemyRate type)
{
	int maxLayerSize = PanelMgr::Instance()->GetMaxLayerNum();

	EnemyGenerateHelper info;
	float setAngle = MyFunc::GetRand(359) * (PI / 180.0f);
	const float lengthAngle = (PI * 2) / Count;
	info.type = QUEEN;
	info.isClockWise = MyFunc::GetRand(1) ? true : false;
	//info.layerNum = LayerNum;
	info.posAngle = setAngle;
	info.layerNum = LayerNum;

	safeSpawn(info);

	//通常の敵の生成
	for (int i = 1; i < Count; i++)
	{
		do
		{
			info.type = (ENEMY_TYPE)MyFunc::GetRand(ENEMY_TYPE_NUM - 1);
		} while (info.type == QUEEN);
		info.type = TypeEnter(type);
		setAngle += lengthAngle;
		if (setAngle >= 2 * PI)
		{
			setAngle -= (2 * PI);
		}
		info.posAngle = setAngle;
		info.isClockWise = MyFunc::GetRand(1) ? true : false;

		safeSpawn(info);
	}
}

void SpawnPattern::Width(int Count, int panelNum, int LayerNum, EnemyRate type)
{
	EnemyGenerateHelper info;


	const int panelSize = PanelMgr::Instance()->GetPanelNum();
	int setPanelNum = panelNum;
	while (setPanelNum >= panelSize)
	{
		setPanelNum -= panelSize;
	}
	auto panel = PanelMgr::Instance()->GetPanelByIndex(setPanelNum);
	float panelStartAngle = panel.get()->GetStartAngle();
	float panelEndAngle = panel.get()->GetEndAngle();
	float align = 0.1f;
	float length = (panelEndAngle - panelStartAngle - align) / Count;
	float setAngle = panelStartAngle + (align / 2);
	//通常の敵の生成
	for (int i = 0; i < Count; i++)
	{


		do
		{
			info.type = (ENEMY_TYPE)MyFunc::GetRand(ENEMY_TYPE_NUM - 1);
		} while (info.type == QUEEN);
		info.type = TypeEnter(type);
		if (i == 0)
		{
			setAngle += length / 2.0f;
		}
		else
		{
			setAngle += length;
		}
		if (setAngle >= 2 * PI)
		{
			setAngle -= (2 * PI);
		}
		info.posAngle = setAngle;
		info.isClockWise = MyFunc::GetRand(1) ? true : false;
		info.layerNum = LayerNum;

		safeSpawn(info);
	}
}

void SpawnPattern::pillar(int Count, int panelNum, EnemyRate type)
{
	vector<int> layerEnemyCount;

	//挿入した数
	int setCount = 0;
	layerEnemyCount.resize(PanelMgr::Instance()->GetMaxLayerNum());

	while (setCount < Count)
	{
		for (int i = 0; i < layerEnemyCount.size(); i++)
		{
			layerEnemyCount[i]++;
			setCount++;
			if (setCount >= Count)
			{
				break;
			}
		}
	}
	for (int i = 0; i < layerEnemyCount.size(); i++)
	{
		Width(layerEnemyCount[layerEnemyCount.size() - i - 1], panelNum, i, type);
	}
}

bool SpawnPattern::safeSpawn(EnemyGenerateHelper info)
{
	if (EnemyMgr::Instance()->GetAliveEnemyNum() >= ENEMY_MAX)
	{
		return false;
	}

	int maxLayerSize = PanelMgr::Instance()->GetMaxLayerNum();
	auto tmpPanel = PanelMgr::Instance()->GetPanelByAngle(info.posAngle, true);
	int pushedLayer = tmpPanel.get()->pushedLayer;
	int nowLayerSize = maxLayerSize - pushedLayer;

	//増えた部分です
	if (info.type == QUEEN)
	{
		float panelStartAngle = tmpPanel.get()->GetStartAngle();
		float panelEndAngle = tmpPanel.get()->GetEndAngle();
		float length = (panelEndAngle - panelStartAngle);
		info.posAngle = panelStartAngle + (length / 2);
	}
	////////////////

	if ((info.layerNum <= nowLayerSize - 1) && (info.layerNum >= 0))
	{
		EnemyMgr::Instance()->Generate(info);
		return true;
	}
	return false;

}

ENEMY_TYPE SpawnPattern::TypeEnter(EnemyRate type)
{
	int maxRate = type.jackRate + type.kingRate + type.queenRate + type.pornRate - 1;

	int rate = GetRand(maxRate);

	if (maxRate == 0)
	{
		return static_cast<ENEMY_TYPE>(GetRand(ENEMY_TYPE_NUM - 1));
	}
	if (rate < type.pornRate)
	{
		return PORN;
	}
	else
	{
		rate -= type.pornRate;
	}

	if (rate < type.queenRate)
	{
		return QUEEN;
	}
	else
	{
		rate -= type.queenRate;
	}

	if (rate < type.kingRate)
	{
		return KING;
	}
	else
	{
		return JACK;
	}
}