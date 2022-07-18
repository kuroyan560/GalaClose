#pragma once
#include"../../Particles.h"
#include"../../Singleton.h"
#include"Constants.h"

namespace EnemyExplosionEffect
{
	struct AlertParticle : public PARTICLE::Particle
	{
		static float3 SET_COLOR;
		static const int LIFE_SPAN = 18;
		int life;

		void Emit(float3 EmitterPos, int ImgIndexX, int ImgIndexY, bool ImgRand = true)override;
		bool isAlive()override;
		void Update()override;
	};

	class AlertParticleMgr : public Particles, public Singleton<AlertParticleMgr>
	{
		friend class Singleton<AlertParticleMgr>;
		AlertParticleMgr();
		void UniqueInit()override;
		void UniqueUpdate()override;
	public:
		void ChangeColor(float3 Color) { AlertParticle::SET_COLOR = Color; }
		void Emit(float3 Pos)override;
	};
};