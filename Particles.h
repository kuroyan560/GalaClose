#pragma once
#include<wrl.h>
#include<d3d12.h>

#include<string>
using namespace std;

#include"MyFunc.h"
using namespace MyDef;

#include<forward_list>

namespace PARTICLE
{
	static const enum ROOT_PARAM
	{
		TEXTURE,
		CONST_BUFF
	};

	struct Const
	{
		XMMATRIX viewProjMat;
		XMMATRIX billboardMat;
		int imgSplitX;
		int imgSplitY;
	};

	struct Vertex
	{
		float3 pos;
		float2 scale = { 1,1 };
		unsigned short index[2]; // �g�p����p�[�e�B�N���摜�̃C���f�b�N�X�ԍ�
		float4 color = { 1,1,1,1 };
	};

	struct Particle
	{
		Vertex vertex;
		~Particle() {};
		virtual void Emit(float3 EmitterPos, int ImgIndexX, int ImgIndexY, bool ImgRand = true) = 0;
		virtual bool isAlive() = 0;
		virtual void Update() = 0;
	};
}

class Particles
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const std::string PIPELINE_KEY;
public:
	static void CreatePipeline();

protected:
	static const int MAX_PARTICLE_NUM = 1024;
	Particles(int ImgHandle, unsigned short X, unsigned short Y);

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	ComPtr<ID3D12Resource> constBuff = nullptr;

	int img; //�摜�n���h��
	unsigned short imgIndex[2];	//�摜������

	//�p�[�e�B�N���|�C���^�i���N���X�j
	PARTICLE::Particle* particles[MAX_PARTICLE_NUM];

	//�ғ����̃p�[�e�B�N�����
	forward_list<PARTICLE::Particle*>particlesList;

	void EmitParticle(int EmitNum, float3 Pos);
	virtual void UniqueInit() = 0;
	virtual void UniqueUpdate() = 0;

public:
	virtual void Emit(float3 Pos) = 0;
	void Init();
	void Update();
	void Draw();
};