#include "EffectMgr.h"
#include"src/users/EnemyKillEvent.h"
#include"src/users/EnemyExplosionEffect.h"

void EffectMgr::Init()
{
	enemyDeadEffect.Init();
	EnemyKillEvent::Instance()->Init();
	EnemyExplosionEffect::AlertParticleMgr::Instance()->Init();
}

void EffectMgr::Update()
{
	enemyDeadEffect.Update();
	EnemyKillEvent::Instance()->Update();
	EnemyExplosionEffect::AlertParticleMgr::Instance()->Update();
}

void EffectMgr::DrawSpriteEffect()
{
	EnemyExplosionEffect::AlertParticleMgr::Instance()->Draw();
}

void EffectMgr::Draw3DEffect()
{
	enemyDeadEffect.Draw();
}

void EffectMgr::EnemyDeadEffect(const float3& Color, float3& EmitPos)
{
	enemyDeadEffect.ChangeColor(Color);
	enemyDeadEffect.Emit(EmitPos);
}
