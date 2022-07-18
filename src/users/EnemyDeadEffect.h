#pragma once
#include"../../PrimitiveParticles.h"

class EnemyDeadEffect
{
	static float3 SET_COLOR;
	class PrimitiveParticle : public PRIMITIVE_PARTICLE::Particle
	{
		float spinSpeed;	//��]����

		float3 emitVec;	//��ԕ���
		float emitSpeed;	//��ԑ���

		int life = 0;	//����

		static const int TO_SMALL_TOTAL_TIME = 30;
		int smallTimer = 0;
		float initRadius;

	public:
		PrimitiveParticle() {};
		void Emit(float3 EmitterPos)override;
		bool isAlive()override;
		void Update()override;
	};

	class PrimitiveParticleMgr : public PrimitivePaticles
	{
		static const int EMIT_PARTICLE_NUM = 30;
		void UniqueInit()override;
		void UniqueUpdate()override;
	public:
		void Emit(float3& Pos)override;
		PrimitiveParticleMgr();
		~PrimitiveParticleMgr();
	};

	PrimitiveParticleMgr effectMgr;

public:
	void Emit(float3& EmitPos);
	void Init();
	void Update();
	void Draw();

	void ChangeColor(const float3& Color) { SET_COLOR = Color; }
};