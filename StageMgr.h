#pragma once
#include<vector>
#include<string>

class Wave;
class EnemyMgr;
class Shoot;
class EventMgr;
class OtoImpact;

class StageMgr
{
	static const std::string DIRECTORY_PATH;

	int stageNum;
	std::string nowStage;	//現在セットされているステージ

	int nowWave;	//現在のウェーブ
	std::vector<Wave*>waves;	//ウェーブ

	int waveFrameCount;	//ウェーブ単位でのフレーム数のカウント

	void ProceedWave(EventMgr* EventMgr,Shoot* Shoot = nullptr);		//次のウェーブへ

	void OnWave();	//現在のウェーブ発動

	EnemyMgr* enemyMgr = nullptr;

	//ウェーブが終わるごとに少しのスパン
	static const int WAVE_SPAN = 60;
	int waveSpanTimer = 0;


public:
	StageMgr(EnemyMgr* EnemyMgr) :enemyMgr(EnemyMgr) {};
	~StageMgr();
	void LoadWaves(int StageNum, std::string StageName);
	void SaveWaves(bool EnemysDie);

	void Init();
	void Update(EventMgr* EventMgr, Shoot* Shoot, OtoImpact* OtoImpact);
	void Finalize();
};