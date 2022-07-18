#include "EnemyBody.h"
#include"Constants.h"
#include"../../Easing.h"

float3 EnemyBody::Particle::SET_COLOR = { 1,1,1 };

void EnemyBody::Particle::Emit(float3 EmitterPos)
{
	//範囲内にエミット
	float angle = MyFunc::GetRand(0, 360);
	vertex.pos = EmitterPos;
	vertex.pos.x += cos(angle) * ENEMY_BODY_EMIT_RANGE_RADIUS;
	vertex.pos.y += sin(angle) * ENEMY_BODY_EMIT_RANGE_RADIUS;

	vertex.color = { SET_COLOR.x,SET_COLOR.y,SET_COLOR.z,1.0f };

	vertex.radius = MyFunc::GetRand(ENEMY_BODY_EMIT_RADIUS_MIN, ENEMY_BODY_EMIT_RADIUS_MAX);
	emitRadius = vertex.radius;

	vertex.rotate.x = MyFunc::GetRand(359);
	vertex.rotate.y = MyFunc::GetRand(359);

	spinSpeed = { MyFunc::GetRandPlusMinus() * ENEMY_BODY_SPIN_SPEED ,MyFunc::GetRandPlusMinus() * ENEMY_BODY_SPIN_SPEED };

	angle = MyFunc::GetRand(359) * PI / 180.0f;
	move = { (float)cos(angle) * ENEMY_BODY_MOVE_SPEED ,(float)sin(angle) * ENEMY_BODY_MOVE_SPEED };
	life = ENEMY_BODY_LIFE_SPAN;
}

bool EnemyBody::Particle::isAlive()
{
	return life /*&& 0 < vertex.radius*/;
}

void EnemyBody::Particle::Update()
{
	//vertex.radius -= ENEMY_BODY_RADIUS_EXPAND_SPEED;
	vertex.radius = Easing::Ease(Out, Sine, (ENEMY_BODY_LIFE_SPAN - life), ENEMY_BODY_LIFE_SPAN, emitRadius, 0.0f);
	vertex.rotate.x += spinSpeed.x;
	vertex.rotate.y += spinSpeed.y;
	vertex.pos.x += move.x;
	vertex.pos.y += move.y;

	life--;
}

void EnemyBody::PrimitiveParticleMgr::UniqueInit()
{

}

void EnemyBody::PrimitiveParticleMgr::UniqueUpdate()
{

}

void EnemyBody::PrimitiveParticleMgr::Emit(float3& Pos)
{
	EmitParticle(MyFunc::GetRand(ENEMY_BODY_EMIT_NUM_MIN, ENEMY_BODY_EMIT_NUM_MAX), Pos);
}

EnemyBody::PrimitiveParticleMgr::PrimitiveParticleMgr()
{
	particles.resize(MAX_PARTICLE_NUM);
	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		itr->reset(new EnemyBody::Particle());
		deadParticles.push_front(*itr);
	}
}

EnemyBody::PrimitiveParticleMgr::~PrimitiveParticleMgr()
{

}