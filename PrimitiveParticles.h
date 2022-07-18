#pragma once
#include<wrl.h>
#include<d3d12.h>

#include<string>
using namespace std;

#include"MyFunc.h"
using namespace MyDef;

#include<forward_list>
#include<memory>


namespace PRIMITIVE_PARTICLE
{
	static const enum ROOT_PARAM
	{
		CAMERA,
	};

	static const enum PRIMITIVE_TYPE
	{
		CUBE,
	};

	struct Vertex
	{
		float3 pos = { 0,0,0 };
		float3 rotate = { 0,0,0 };
		float radius = 1;
		float4 color = { 1,1,1,1 };
		unsigned short primitiveType = CUBE;
	};

	struct Particle
	{
		Vertex vertex;
		~Particle() {};
		virtual void Emit(float3 EmitterPos) = 0;
		virtual bool isAlive() = 0;
		virtual void Update() = 0;
	};
}

class PrimitivePaticles
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const std::string PIPELINE_KEY;
public:
	static void CreatePipeline();

protected:
	static const int MAX_PARTICLE_NUM = 2048;
	PrimitivePaticles();

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	//パーティクルインスタンス（基底クラス）
	forward_list<std::shared_ptr<PRIMITIVE_PARTICLE::Particle>>particles;

	//稼働中のパーティクル情報
	forward_list<std::weak_ptr<PRIMITIVE_PARTICLE::Particle>>aliveParticles;
	//死亡中のパーティクル情報
	forward_list<std::weak_ptr<PRIMITIVE_PARTICLE::Particle>>deadParticles;

	void EmitParticle(int EmitNum, float3& Pos);
	virtual void UniqueInit() = 0;
	virtual void UniqueUpdate() = 0;

public:
	virtual void Emit(float3& Pos) = 0;
	void Init();
	void Update();
	void Draw();
};

