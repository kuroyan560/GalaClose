#pragma once
#include"../../PrimitiveParticles.h"
#include"../../Singleton.h"

namespace EnemyBody
{
	struct Particle : public PRIMITIVE_PARTICLE::Particle
	{
		static float3 SET_COLOR;
		//static const float EMIT_RANGE_RADIUS; //î≠ê∂à íuÇÃîºåa
		//static const float MAX_SIZE;
		//static const float MIN_SIZE;
		float2 spinSpeed;
		int life;
		float emitRadius;
		float2 move;

	public:
		void Emit(float3 EmitterPos)override;
		bool isAlive()override;
		void Update()override;
	};
	struct PrimitiveParticleMgr : public PrimitivePaticles, public Singleton<PrimitiveParticleMgr>
	{
		friend class Singleton<PrimitiveParticleMgr>;
		PrimitiveParticleMgr();

		void UniqueInit()override;
		void UniqueUpdate()override;
	public:
		void ChangeColor(const float3& Color) { Particle::SET_COLOR = Color; }
		void Emit(float3& Pos)override;
		~PrimitiveParticleMgr();
	};
};