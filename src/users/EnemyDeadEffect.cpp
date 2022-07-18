#include "EnemyDeadEffect.h"
#include"Constants.h"
#include"../../Easing.h"

float3 EnemyDeadEffect::SET_COLOR = { 1,1,1 };

void EnemyDeadEffect::PrimitiveParticle::Emit(float3 EmitterPos)
{
	vertex.pos = EmitterPos;
	vertex.rotate.x = MyFunc::GetRand(360);
	vertex.rotate.y = MyFunc::GetRand(360);
	vertex.radius = MyFunc::GetRand(ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MIN, ENEMY_DEAD_PRIMITIVE_PARTICLE_RADIUS_RAND_MAX);
	initRadius = vertex.radius;
	vertex.color = { EnemyDeadEffect::SET_COLOR.x,EnemyDeadEffect::SET_COLOR.y,EnemyDeadEffect::SET_COLOR.z,1.0f };

	spinSpeed = MyFunc::GetRand(ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MIN, ENEMY_DEAD_PRIMITIVE_PARTICLE_SPIN_RAND_MAX);
	spinSpeed *= MyFunc::GetRandPlusMinus();

	emitVec.x = cos(PI / 180 * MyFunc::GetRand(359));
	emitVec.y = cos(PI / 180 * MyFunc::GetRand(359));
	emitVec.z = cos(PI / 180 * MyFunc::GetRand(359));
	emitSpeed = MyFunc::GetRand(ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MIN, ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_RAND_MAX);

	life = ENEMY_DEAD_PRIMITIVE_PARTICLE_LIFE_SPAN;
	smallTimer = TO_SMALL_TOTAL_TIME;
}

bool EnemyDeadEffect::PrimitiveParticle::isAlive()
{
	return life || smallTimer;
}

void EnemyDeadEffect::PrimitiveParticle::Update()
{
	vertex.pos = MyFunc::AddXMFLOAT3(vertex.pos, MyFunc::MulXMFLOAT3(emitVec, emitSpeed));
	vertex.rotate.x += spinSpeed;
	vertex.rotate.y += spinSpeed;
	emitSpeed *= ENEMY_DEAD_PRIMITIVE_PARTICLE_SPEED_ATTENUATION_RATE;

	if (life)
	{
		life--;
	}
	else
	{
		smallTimer--;
		vertex.radius = Easing::Ease(In, Back, TO_SMALL_TOTAL_TIME - smallTimer, TO_SMALL_TOTAL_TIME, initRadius, 0.0f);
	}
}

void EnemyDeadEffect::PrimitiveParticleMgr::UniqueInit()
{
}

void EnemyDeadEffect::PrimitiveParticleMgr::UniqueUpdate()
{
}

void EnemyDeadEffect::PrimitiveParticleMgr::Emit(float3& Pos)
{
	PrimitivePaticles::EmitParticle(EMIT_PARTICLE_NUM, Pos);
}

EnemyDeadEffect::PrimitiveParticleMgr::PrimitiveParticleMgr()
{
	particles.resize(MAX_PARTICLE_NUM);
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->reset(new PrimitiveParticle());
		deadParticles.push_front(*itr);
	}
}

EnemyDeadEffect::PrimitiveParticleMgr::~PrimitiveParticleMgr()
{
}

void EnemyDeadEffect::Emit(float3& EmitPos)
{
	effectMgr.Emit(EmitPos);
}

void EnemyDeadEffect::Init()
{
	effectMgr.Init();
}

void EnemyDeadEffect::Update()
{
	effectMgr.Update();
}

void EnemyDeadEffect::Draw()
{
	effectMgr.Draw();
}