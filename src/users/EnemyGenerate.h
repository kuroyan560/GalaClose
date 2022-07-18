#pragma once
#include "EnemyGenerateHelper.h"
#include "StageGenerateHelper.h"
#include <vector>
#include"../../Singleton.h"

enum SPAWNPATTERN
{
	RANDUM,
	CIRCLE,
	WIDTH,
	PILLAR
};

//�o������G��ނ̊���
struct EnemyRate
{
	int pornRate = 10;
	int queenRate = 0;
	int kingRate = 10;
	int jackRate = 0;
};

//�o���p�^�[���̊���
struct PatternRate
{
	int randumRate = 1;
	int circleRate = 1;
	int widthRate = 1;
	int pillarRate = 5;

};

struct scoreModeGeneratepattern
{
	//�G�̏o������
	EnemyRate enemyRate;
	//�G�̐������x
	int maxTimer = 300;
	//�G�̐�����
	int count = 3;
	//�p�^�[���̏o������
	PatternRate patternRate;
};
class EnemyGenerate : public Singleton<EnemyGenerate>
{
	friend class Singleton<EnemyGenerate>;
public:
	bool load = false;
	//void Init();
	void PanelsInit();
	void EnemysInit();
	void Update();
	void Finalize();
	void Draw();

	void LoadFile(const char *fileName);

	const std::vector<EnemyGenerateHelper>& EnemyOutput();
	const StageGenerateHelper& StageOutput();
	void scoreModeGenerate();

private:
	EnemyGenerate();
	StageGenerateHelper stageData;

	scoreModeGeneratepattern generatePattern;
	void SetGeneratePattern(scoreModeGeneratepattern info);
	int timer = 0;

};

namespace SpawnPattern
{
	/// <summary>
	/// ��O�ɐ������ꂻ���Ȃ琶�����Ȃ�
	/// </summary>
	/// <param name="info">�������悤�Ƃ��Ă�G�̏��</param>
	/// <returns>����������true����ȊO�Ȃ�false</returns>
	bool safeSpawn(EnemyGenerateHelper info);
	/// <summary>
	/// ���S�ȃ����_���Ő�������
	/// </summary>
	/// <param name="Count">��������G�̐�</param>
	/// <param name="type">�G�̎��</param>
	void Randum(int Count, EnemyRate type);

	/// <summary>
	/// ��̃��C���[�̉~����ɓG�𓙊Ԋu�ɐ�������
	/// </summary>
	/// <param name="Count">��������G�̐�</param>
	/// <param name="LayerNum">���܂�Ă��郌�C���[</param>
	void Ring(int Count, int LayerNum, EnemyRate type);

	/// <summary>
	/// �G�����ɐ��܂��
	/// </summary>
	/// <param name="Count">���������鐔</param>
	/// <param name="LayerNum">���܂�Ă��郌�C���[</param>
	/// <param name="type">���܂�Ă���G�̃^�C�v</param>
	void Width(int Count, int panelNum, int LayerNum, EnemyRate type);

	/// <summary>
	/// �Ă����c�꒼���ɐ��܂��
	/// </summary>
	/// <param name="Count">��</param>
	/// <param name="panelNum">�ǂ̃p�l���ɐ��܂�邩</param>
	/// <param name="type">type</param>
	void pillar(int Count, int panelNum, EnemyRate type);

	ENEMY_TYPE TypeEnter(EnemyRate type);
}
