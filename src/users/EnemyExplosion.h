#pragma once
#include<memory>
#include<DirectXMath.h>

enum ENEMY_TYPE;
class Model_HSM_Component;

class EnemyExplosion
{
	static const enum STATUS { DEFAULT, ATTENTION, WARNING, DANGER, STATUS_NUM };
	static const int FLASH_SPAN[STATUS_NUM];

	STATUS status;
	int timer;
	int lifeSpan;
	DirectX::XMFLOAT3* enemyPos;

	int flashTimer;
	ENEMY_TYPE enemyType;

	bool on;

	DirectX::XMFLOAT2 shakeRand = { 0.0f,0.0f };

public:
	EnemyExplosion(DirectX::XMFLOAT3* EnemyPos);
	void Reset(const ENEMY_TYPE& Type, bool On = true);	//エネミー生成時
	void Update();

	void AffectComponent(Model_HSM_Component& Component);
	const DirectX::XMFLOAT2 GetShakeRand() { return shakeRand; }
};

