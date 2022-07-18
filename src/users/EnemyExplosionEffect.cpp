#include "EnemyExplosionEffect.h"
#include"../../Easing.h"
#include"../../SRVDescMgr.h"

float3 EnemyExplosionEffect::AlertParticle::SET_COLOR = { 1.0f,1.0f,1.0f };

void EnemyExplosionEffect::AlertParticle::Emit(float3 EmitterPos, int ImgIndexX, int ImgIndexY, bool ImgRand)
{
	vertex.pos = EmitterPos;
	vertex.index[0] = 0;
	vertex.index[1] = 0;
	vertex.scale = { 0.0f,0.0f };
	vertex.color.w = 1.0f;
	vertex.color.x = SET_COLOR.x;
	vertex.color.y = SET_COLOR.y;
	vertex.color.z = SET_COLOR.z;

	life = 0;
}

bool EnemyExplosionEffect::AlertParticle::isAlive()
{
	return life != LIFE_SPAN;
}

void EnemyExplosionEffect::AlertParticle::Update()
{
	vertex.scale = Easing::Ease(Out, Quart, life, LIFE_SPAN, { 0.0f,0.0f }, { 10.0f,10.0f });
	vertex.color.w = Easing::Ease(In, Quart, life, LIFE_SPAN, 1.0f, 0.0f);

	if (life < LIFE_SPAN)
	{
		life++;
	}
}

EnemyExplosionEffect::AlertParticleMgr::AlertParticleMgr()
	:Particles(SRVDescMgr::Instance()->CreateSRV("resource/img/enemyAlert.png"), 1, 1)
{
	for (int i = 0; i < MAX_PARTICLE_NUM; ++i)
	{
		particles[i] = new AlertParticle();
	}
}

void EnemyExplosionEffect::AlertParticleMgr::UniqueInit()
{
}

void EnemyExplosionEffect::AlertParticleMgr::UniqueUpdate()
{
}

void EnemyExplosionEffect::AlertParticleMgr::Emit(float3 Pos)
{
	EmitParticle(1, Pos);
}
