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
	std::string nowStage;	//���݃Z�b�g����Ă���X�e�[�W

	int nowWave;	//���݂̃E�F�[�u
	std::vector<Wave*>waves;	//�E�F�[�u

	int waveFrameCount;	//�E�F�[�u�P�ʂł̃t���[�����̃J�E���g

	void ProceedWave(EventMgr* EventMgr,Shoot* Shoot = nullptr);		//���̃E�F�[�u��

	void OnWave();	//���݂̃E�F�[�u����

	EnemyMgr* enemyMgr = nullptr;

	//�E�F�[�u���I��邲�Ƃɏ����̃X�p��
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