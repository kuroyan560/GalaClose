#pragma once
#include"Singleton.h"
#include"src/users/EnemyDeadEffect.h"

class EffectMgr : public Singleton<EffectMgr>
{
	friend class Singleton<EffectMgr>;
	EffectMgr() {};

	EnemyDeadEffect enemyDeadEffect;

public:
	~EffectMgr() {};

	void Init();
	void Update();
	void DrawSpriteEffect();
	void Draw3DEffect();

	void EnemyDeadEffect(const float3& Color,float3& EmitPos);
};