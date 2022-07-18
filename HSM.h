#pragma once
#include<d3d12.h>
#include<wrl.h>

#include<map>
#include<vector>
#include<string.h>
using namespace std;

#include"MyFunc.h"
using namespace MyDef;

#include"BoneTree.h"
#include"DirtyFlag.h"

namespace HSM
{
	static const string EXTENSION = ".hsm";

	static const enum CAN_LOAD_MODEL_FORMAT { PMX, MODEL_FORMAT_NUM };
	static const string MODEL_EXTENSIONS_STR[CAN_LOAD_MODEL_FORMAT::MODEL_FORMAT_NUM]
		= { "pmx" };
	static const enum INDEX_BYTE_SIZE
	{
		VERTEX_SIZE,
		TEXTURE_SIZE,
		MATERIAL_SIZE,
		BONE_SIZE,
		MORPH_SIZE,
		RIGIDBODY_SIZE,
		TYPE_SIZE_NUM,
	};

	static const enum ROOT_PARAM
	{
		MASTER_COLOR,
		MATERIAL,
		NORMAL_TEX,
		SPH_TEX,
		SPA_TEX,
		TOON_TEX,
		CAMERA,
		WORLD_MAT,
		BONE_MAT
	};

	struct Header
	{
		string name;
		char byteSize[INDEX_BYTE_SIZE::TYPE_SIZE_NUM] = { 2 };
	};

	struct Vertex
	{
		float3 pos;
		float3 normal;
		float2 uv;
		signed short boneIndex[4] = { -1,-1,-1,-1 };
		float boneWeight[4] = { 0.0f,0.0f,0.0f,0.0f };
	};

	struct Mesh
	{
		string name;
		int materialIndex;
		int vertexNum;
	};

	struct Bone
	{
		string name;
		unsigned char parent = -1;	//�e�{�[��
		int transLayer = 0;	//�ό`�K�w
		float3 pos = { 0.0f,0.0f,0.0f };
		XMMATRIX offsetMat = XMMatrixIdentity();
		//short flag;	//16�r�b�g
		/*
			�ڑ���A��]�\�A�ړ��\�A�\���A����AIK�A���[�J���t�^�A��]�t�^
		*/
	};

	//�{�[���P�{�P�{�̍\����
	struct BoneTransform
	{
		DirtyFlag dirty;
		float3 pos = { 0,0,0 };
		float3 scale = { 1,1,1 };
		float3 rotate = { 0,0,0 };
		XMMATRIX* mat = nullptr;

		BoneTransform();
		void DirtyRegister() { dirty.Register(); }
		bool TransformAffectToMat();	//�ω�����������true
	};

	struct Skeleton;
	//�P�̃��f���̃{�[�����܂Ƃ߂�\����
	//���̒P��
	struct BoneGroup
	{
	private:
		void RecursiveMatrixMultiply(BoneNode* Node, const XMMATRIX& Mat);
	public:
		Skeleton* skeleton = nullptr;	//�Ή����郂�f���̃X�P���g�����
		Microsoft::WRL::ComPtr<ID3D12Resource>boneBuff = nullptr;
		vector<BoneTransform>boneTransform;
		vector<XMMATRIX>boneMatrices;

		int nowAnimation;	//�K�p����Ă���A�j���[�V�����̐�

		BoneGroup(Skeleton* Skeleton);
		void SetBoneConstBuff(Skeleton* ModelSkeleton,int RootParam);
	};

	//�{�[���𓮂����i�|�[�Y�����j���߂̏����܂Ƃ߂��\����
	//�����f���P��
	struct Skeleton
	{
		string modelName;
		std::vector<HSM::Bone>bones;
		std::map<std::string, BoneNode> boneNodeTable;	//�{�[���m�[�h�e�[�u��

		int GetIndex(std::string BoneName);
	};

	struct MasterColor
	{
		float4 color = { 1,1,1,1 }; //RGBA
		bool shadow = true;
		float flash = 0.0f;
	};

	struct Transform
	{
		float3 pos = { 0,0,0 };
		float3 scale = { 1,1,1 };
		float3 rotate = { 0,0,0 };
		float3 up = { 0,1,0 };	//��x�N�g��
		float3 front = { 0,0,1 };	//�O�x�N�g��
		XMMATRIX mat = XMMatrixIdentity();
	};
}