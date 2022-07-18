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
		unsigned char parent = -1;	//親ボーン
		int transLayer = 0;	//変形階層
		float3 pos = { 0.0f,0.0f,0.0f };
		XMMATRIX offsetMat = XMMatrixIdentity();
		//short flag;	//16ビット
		/*
			接続先、回転可能、移動可能、表示、操作可、IK、ローカル付与、回転付与
		*/
	};

	//ボーン１本１本の構造体
	struct BoneTransform
	{
		DirtyFlag dirty;
		float3 pos = { 0,0,0 };
		float3 scale = { 1,1,1 };
		float3 rotate = { 0,0,0 };
		XMMATRIX* mat = nullptr;

		BoneTransform();
		void DirtyRegister() { dirty.Register(); }
		bool TransformAffectToMat();	//変化があったらtrue
	};

	struct Skeleton;
	//１つのモデルのボーンをまとめる構造体
	//※個体単位
	struct BoneGroup
	{
	private:
		void RecursiveMatrixMultiply(BoneNode* Node, const XMMATRIX& Mat);
	public:
		Skeleton* skeleton = nullptr;	//対応するモデルのスケルトン情報
		Microsoft::WRL::ComPtr<ID3D12Resource>boneBuff = nullptr;
		vector<BoneTransform>boneTransform;
		vector<XMMATRIX>boneMatrices;

		int nowAnimation;	//適用されているアニメーションの数

		BoneGroup(Skeleton* Skeleton);
		void SetBoneConstBuff(Skeleton* ModelSkeleton,int RootParam);
	};

	//ボーンを動かす（ポーズを取る）ための情報をまとめた構造体
	//※モデル単位
	struct Skeleton
	{
		string modelName;
		std::vector<HSM::Bone>bones;
		std::map<std::string, BoneNode> boneNodeTable;	//ボーンノードテーブル

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
		float3 up = { 0,1,0 };	//上ベクトル
		float3 front = { 0,0,1 };	//前ベクトル
		XMMATRIX mat = XMMatrixIdentity();
	};
}