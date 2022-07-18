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

//出現する敵種類の割合
struct EnemyRate
{
	int pornRate = 10;
	int queenRate = 0;
	int kingRate = 10;
	int jackRate = 0;
};

//出現パターンの割合
struct PatternRate
{
	int randumRate = 1;
	int circleRate = 1;
	int widthRate = 1;
	int pillarRate = 5;

};

struct scoreModeGeneratepattern
{
	//敵の出現割合
	EnemyRate enemyRate;
	//敵の生成速度
	int maxTimer = 300;
	//敵の生成量
	int count = 3;
	//パターンの出現割合
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
	/// 場外に生成されそうなら生成しない
	/// </summary>
	/// <param name="info">生成しようとしてる敵の情報</param>
	/// <returns>生成したらtrueそれ以外ならfalse</returns>
	bool safeSpawn(EnemyGenerateHelper info);
	/// <summary>
	/// 完全なランダムで生成する
	/// </summary>
	/// <param name="Count">生成する敵の数</param>
	/// <param name="type">敵の種類</param>
	void Randum(int Count, EnemyRate type);

	/// <summary>
	/// 一つのレイヤーの円周上に敵を等間隔に生成する
	/// </summary>
	/// <param name="Count">生成する敵の数</param>
	/// <param name="LayerNum">生まれてくるレイヤー</param>
	void Ring(int Count, int LayerNum, EnemyRate type);

	/// <summary>
	/// 敵が扇状に生まれる
	/// </summary>
	/// <param name="Count">数生成する数</param>
	/// <param name="LayerNum">生まれてくるレイヤー</param>
	/// <param name="type">生まれてくる敵のタイプ</param>
	void Width(int Count, int panelNum, int LayerNum, EnemyRate type);

	/// <summary>
	/// てきが縦一直線に生まれる
	/// </summary>
	/// <param name="Count">数</param>
	/// <param name="panelNum">どのパネルに生まれるか</param>
	/// <param name="type">type</param>
	void pillar(int Count, int panelNum, EnemyRate type);

	ENEMY_TYPE TypeEnter(EnemyRate type);
}
