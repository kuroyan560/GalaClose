#include "StageMgr.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include<stdlib.h>

#include"WinApp.h"
#include"MyFunc.h"
#include"EnemyMgr.h"

#include<direct.h>
#include"Master.h"
#include"EnemyInfo.h"

#include"Shoot.h"
#include"EnemyMgr.h"
#include"Wave.h"
#include"EventMgr.h"
#include"EnemyBullet.h"
#include"OtoImpact.h"

const std::string StageMgr::DIRECTORY_PATH = "resource/stage/";
void StageMgr::ProceedWave(EventMgr* EventMgr, Shoot* Shoot)
{
	if (nowWave < waves.size() - 1)
	{
		if (Shoot != nullptr)
		{
			Shoot->Init();
		}
		nowWave++;
		OnWave();
	}
	else
	{

		if (EventMgr->BossAppearEvent(stageNum))
		{
			if (Shoot != nullptr)
			{
				Shoot->Init();
			}
		}
		//OnWave();
	}
}

void StageMgr::OnWave()
{
	waveFrameCount = 0;
	enemyMgr->EnemysAllDie();
	waves[nowWave]->AppearEnemys(enemyMgr);
	waveSpanTimer = WAVE_SPAN;
}

StageMgr::~StageMgr()
{
	SaveWaves(true);
	for (int i = 0; i < waves.size(); i++)
	{
		MyFunc::SafetyDelete(waves[i]);
	}
}

void StageMgr::LoadWaves(int StageNum, std::string StageName)
{
	stageNum = StageNum;

	//既にセットされているステージなら何もしない
	if (nowStage == StageName)return;

	if (!waves.empty())waves.clear();

	//resource/stage/(StageNum)/
	std::string pass = DIRECTORY_PATH + StageName + '/';

	std::ifstream file;
	file.open(pass + "waves.txt");
	if (!file.fail())
	{
		std::string line;
		while (getline(file, line))
		{
			std::istringstream line_stream(line);

			std::string key;
			getline(line_stream, key, ' ');

			//新規ウェーブ
			if (key == "newwav")
			{
				getline(file, line);
				int frame = atoi(line.c_str());
				waves.push_back(new Wave(frame));
			}
			//新規敵
			else if (key == "newenemy")
			{
				//敵の種類
				getline(file, line);
				ENEMY_TYPE type = (ENEMY_TYPE)(atoi(line.c_str()));

				waves.back()->AddEnemyInfo(type, StageName, waves.size() - 1);
			}
			//新規shot情報(発射と着弾のタイミング)
			else if (key == "newshot")
			{
				getline(file, line);
				std::istringstream s(line);
				ShotInfo info;
				int tmp;
				s >> tmp;
				info.type = (ENEMY_BULLET_TYPE)tmp;
				s >> info.shotFrame;
				s >> info.untilHitFrame;
				waves.back()->enemysInfo.back()->shotInfo.emplace_back(info);
			}
			//新規tackle情報
			else if (key == "newtackle")
			{
				getline(file, line);
				std::istringstream s(line);
				s >> waves.back()->enemysInfo.back()->tackleFrame;
				s >> waves.back()->enemysInfo.back()->untilHitFrame;
			}
		}
	}
	file.close();
	nowStage = StageName;
}

void StageMgr::SaveWaves(bool EnemysDie)
{
	std::string str;

	for (int i = 0; i < waves.size(); i++)
	{
		str += "newwav\n";
		str += to_string(waves[i]->waitFrame) + '\n';
	
		for (auto itr = waves[i]->enemysInfo.begin(); itr != waves[i]->enemysInfo.end(); itr++)
		{
			str += "newenemy\n";
			str += to_string((int)(*itr)->type)/* + ' ' + waves[i]->enemysInfo[num]->move->GetName() */+ '\n';
			if (EnemyInfo::GetAttackWay((*itr)->type) == BULLET)
			{
				for (auto it = (*itr)->shotInfo.begin(); it != (*itr)->shotInfo.end(); ++it)
				{
					str += "newshot\n";
					str += to_string((int)it->type) + ' ' + to_string(it->shotFrame) + ' ' + to_string(it->untilHitFrame) + '\n';
				}
			}
			else if (EnemyInfo::GetAttackWay((*itr)->type) == TACKLE)
			{
				str += "newtackle\n";
				str += to_string((*itr)->tackleFrame) + ' ' + to_string((*itr)->untilHitFrame) + '\n';
			}
		}

		std::string dir = this->nowStage + '/';
		waves[i]->SaveWave(&dir, EnemysDie);
	}

	if (!str.empty())
	{
		std::ofstream file;
		file.open(StageMgr::DIRECTORY_PATH + nowStage + '/' + "waves.txt");
		if (!file.fail())
		{
			file << str;
			file.close();
		}
	}
	else
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "中身空っぽだったから保存しないよ！");
	}
}

void StageMgr::Init()
{
	if (!nowStage.empty())
	{
		nowWave = 0;
		waveFrameCount = 0;
		OnWave();
	}
}

void StageMgr::Update(EventMgr* EventMgr, Shoot* Shoot, OtoImpact* OtoImpact)
{
	if (!Master::Instance()->DebugMode())
	{
		if (waveFrameCount != waves[nowWave]->waitFrame)
		{
			waveFrameCount++;
		}
	}
	if (!waves.empty())
	{
		if ((enemyMgr->AllDead() && EnemyBullet::NUM == 0) /*|| waveFrameCount == waves[nowWave]->waitFrame*/)
		{
			if (waveSpanTimer && !OtoImpact->NowStan())
			{
				waveSpanTimer--;
			}
			else
			{
				ProceedWave(EventMgr, Shoot);
			}
		}
	}
}

void StageMgr::Finalize()
{
	nowWave = 0;
	waveFrameCount = 0;
	OnWave();
}