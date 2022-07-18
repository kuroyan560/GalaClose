#pragma once
#include<DirectXMath.h>
#include<memory>
#include"GameObjectInterFace.h"
#include"Panel.h"
#include"../../Model_HSM.h"
#include"EnemyGenerateHelper.h"
#include"EnemyExplosion.h"

class Enemy : public GameObjectInterFace
{
public:
	static int ENEMY_KILL_SE;
	static void LoadAudios();
	Enemy();

	/// <summary>
	/// 初期化、スポーンさせる際に使う
	/// </summary>
	/// <param name="isClockWise">向き</param>
	/// <param name="posAngle">現在位置</param>
	/// <param name="panelIndex">現在所属するパネル</param>
	/// <param name="layer">現在いる階層</param>
	/// <param name="panelType">色などの属性</param>
	/// <param name="speedRate">速度倍率</param>
	/// 
	virtual void UniqueInit() {};
	virtual void UniqueUpdate();
	virtual void UniqueFinalize() {};
	void Init()override;
	void Update()override;
	void Finalize()override;
	virtual void Draw()override;

	//階層が下がる(階層がゼロの時に呼び出すと死ぬ)
	void Down(const int& PushedNum);


	//生存フラグをtrueにする
	virtual void Spawn(const EnemyGenerateHelper& InitData);

	//死ぬ
	virtual void Dead();

	void Collision();

	//キャラクターの属性返す
	char GetType();

	//現在所属するパネルの番号を返す
	int GetPanelIndex();

	const int &Getlayer();

	const float &GetAngle();

	Model_HSM_Component& GetComponent() { return component; }
	const bool& GetIsAlive() { return isAlive; }

	std::weak_ptr<Panel> GetPanel() { return panel; }
public://デバッグ用の雪駄
	//void SetLayer(int num);
	void SetLayer(int LayerNum);

	//void SetType(char num);
	const ENEMY_TYPE& GetEnemyType() { return type; }

private://定数

	////1°
	//static const float baseSpeed;
	////360°
	//static const float round;
	////レイヤーごとの大きさ
	//static const float offsetLength;

private://静的メンバ変数
	static float baseAngle;
protected://メンバ変数

	ENEMY_TYPE type = PORN;

	EnemyExplosion enemyExplosion;
	//どっち回りか
	bool isClockWise;

	//ステージの中心から度の向きにいるか
	float posAngle;

	//どの向きのパネルにいるか
	std::weak_ptr<Panel> panel;

	//どの階層にいるか
	//階層の番号
	float layerNum;

	//移動速度
	float moveSpeed;

	//生きているか
	bool isAlive;

	//現在の自分の属性
	//char type;

	//DirectX::XMFLOAT3 drawPos;
	Model_HSM_Component component;

	int bodyEmitTimer;
	bool bodyEmitFlg = true;
	bool beDrained = false;

	//スポーン演出
	int spawnTimer;
	float defaultScale;

	//回転移動時に境目に接触したかの判定
	bool IsOutRange(float startAngle, float EndAngle);

	//反対を向く時に呼び出す
	void Reverce();

	//特定のパネルを目指す
	void panelVisit(const int queenNum);

	void CheckQueen();


	/// <summary>
	/// どちらが近いか方角を決定する
	/// </summary>
	/// <param name="startAngle">時計回り側側</param>
	/// <param name="endAngle">反時計回り側</param>
	/// <returns>時計回りならstartが近かったらtrueそれ以外ならfalse</returns>

	//二点間の距離の短い方のアングルをとる
	float angleLength(float a, float b);
};