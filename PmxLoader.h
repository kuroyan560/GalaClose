#pragma once
#include<vector>
#include<string>
#include<list>
using namespace std;

#include"MyFunc.h"
using namespace MyDef;

#include"Model_HSM.h"

namespace PMX
{
	static const enum WEIGHT_TRANS_METHOD { BDEF1, BDEF2, BDEF4, SDEF };
	static const int PMX_BYTE_ARRAY_INDEX_SIZE_OFFSET = 2;

	struct PMXHeader	//PMXヘッダ
	{
		unsigned char format[4]; //"PMX "
		float version; //ver (2.0/2.1)
		vector<unsigned char> byteArray;

		/*バイト列 - byte
			[0] - エンコード方式 | 0:UTF16 1 : UTF8
			[1] - 追加UV数 | 0～4 詳細は頂点参照

			[2] - 頂点Indexサイズ | 1, 2, 4 のいずれか
			[3] - テクスチャIndexサイズ | 1, 2, 4 のいずれか
			[4] - 材質Indexサイズ | 1, 2, 4 のいずれか
			[5] - ボーンIndexサイズ | 1, 2, 4 のいずれか
			[6] - モーフIndexサイズ | 1, 2, 4 のいずれか
			[7] - 剛体Indexサイズ | 1, 2, 4 のいずれか*/
	};
	struct PMXInfo	//モデル情報
	{
		string modelName_JP;
		string modelName_EN;
		string comment_JP;
		string comment_EN;
	};

	struct PMXVertex //頂点データ
	{
		PMXVertex(size_t BoneIndexSize);
		~PMXVertex();
		PMXVertex(PMXVertex&& Tmp);

		float3 pos;
		float3 normal;
		float2 uv;
		vector<float4>additionalUv;	//追加UV（ヘッダで指定される　0 ~ 4個）　※できれば無いほうが良い

		unsigned char weightTransMethod;		//ウェイト変形方式 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF
		unsigned char* boneIndex[4] = { nullptr };	//参照するボーンのIndex
		float boneWeight[4] = { 0.0f };	//ウェイト
		float3 SDEF_C;
		float3 SDEF_R0;
		float3 SDEF_R1;

		float edgeRate;	//エッジ倍率  材質のエッジサイズに対しての倍率値
	/*  BDEF1: int | 4 | ボーンのみ
		BDEF2 : int, int, float | 4 * 3 | ボーン2つと、ボーン1のウェイト値(PMD方式)
		BDEF4 : int* 4, float* 4 | 4 * 8 | ボーン4つと、それぞれのウェイト値。ウェイト合計が1.0である保障はしない
		SDEF : int, int, float, float3 * 3
		| 4 * 12 | BDEF2に加え、SDEF用のfloat3(Vector3)が3つ。実際の計算ではさらに補正値の算出が必要(一応そのままBDEF2としても使用可能)*/
	};

	struct PMXSurface
	{
		unsigned char* vertIndex[3];

		PMXSurface(size_t Size);
		~PMXSurface();
		PMXSurface(PMXSurface&& Tmp);
	};

	struct PMXMaterial
	{
		PMXMaterial(size_t TexIndexSize);
		~PMXMaterial();
		PMXMaterial(PMXMaterial&& Tmp);
		string materialName_JP;
		string materialName_EN;
		float4 diffuse;
		float3 specular;
		float specular_c;	//スペキュラー係数( c は coefficient の頭文字)
		float3 ambient;
		unsigned char bitFlag;	//描画フラグ(8bit) - 各bit 0:OFF 1:ON
		/*	0x01:両面描画, 0x02:地面影, 0x04:セルフシャドウマップへの描画, 0x08:セルフシャドウの描画, 0x10:エッジ描画 */
		float4 edgeColor;
		float edgeSize;

		unsigned char* normalTexIndex;	//通常テクスチャ, テクスチャテーブルの参照Index
		unsigned char* sphereTexIndex;	//スフィアテクスチャ, テクスチャテーブルの参照Index  ※テクスチャ拡張子の制限なし

		//スフィアモード 0:無効 1:乗算(sph) 2:加算(spa) 3:サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
		unsigned char sphereMode;
		unsigned char shareToonFlag; //共有Toonフラグ 0:継続値は個別Toon 1:継続値は共有Toon

		//※どちらか一方↓
		//共有フラグ０の場合
		unsigned char* toonTexIndex;	//Toonテクスチャ, テクスチャテーブルの参照Index
		//共有フラグ１の場合
		unsigned char* toonTex; //共有Toonテクスチャ[0～9] -> それぞれ toon01.bmp～toon10.bmp に

		string memo; //メモ : 自由欄／スクリプト記述／エフェクトへのパラメータ配置など

		unsigned int forSurfaceNum; // 材質に対応する面(頂点)数 (必ず3の倍数になる)
	};

	namespace Bone
	{
		struct PMXBoneInfo
		{
		protected:
			PMXBoneInfo() {};

		public:
			virtual ~PMXBoneInfo() {};
		};

		//接続先(PMD子ボーン指定)表示方法 -> 0:座標オフセットで指定 1:ボーンで指定
		struct Bone_0x0001 : public PMXBoneInfo
		{
		protected:
			Bone_0x0001() {};
		};
		struct PosOffset_0x0001:public Bone_0x0001 //座標オフセット, ボーン位置からの相対分 ：０
		{
			float3 posOffset;
		};
		
		struct Index_0x0001 :public Bone_0x0001 //接続先ボーンのボーンIndex　：１
		{
			unsigned char* boneIndex;
			Index_0x0001(size_t BoneIndexSize);
			~Index_0x0001();
			Index_0x0001(Index_0x0001&& Tmp);
		};

		//回転付与:1 または 移動付与:1 の場合
		struct Grant : public PMXBoneInfo
		{
			unsigned char* boneIndex; //接続先ボーンのボーンIndex
			float rate; //付与率
			Grant(size_t BoneIndexSize);
			~Grant();
			Grant(Grant&& Tmp);
		};

		//軸固定:1 の場合
		struct AxisVec : public PMXBoneInfo
		{
			float3 vec; //軸固定:1 の場合
		};

		//ローカル軸:1 の場合
		struct LocalAxisVec : public PMXBoneInfo
		{
			float3 xVec;
			float3 zVec;
		};

		//外部親変形:1 の場合
		struct Key : public PMXBoneInfo
		{
			unsigned int key;
		};

		//IK:1 の場合 IKデータを格納
		struct IK : public PMXBoneInfo
		{
			IK(size_t BoneIndexSize);
			~IK();
			IK(IK&& Tmp);
			unsigned char* boneIndex; //IKターゲットボーンのボーンIndex
			int loopTimes; //IKループ回数 (PMD及びMMD環境では255回が最大になるようです)
			float limitAngle; // IKループ計算時の1回あたりの制限角度 -> ラジアン角 | PMDのIK値とは4倍異なるので注意
			int linkNum; //IKリンク数 : 後続の要素数

			struct IKLink
			{
				IKLink(size_t BoneIndexSize);
				~IKLink();
				IKLink(IKLink&& Tmp);
				unsigned char* boneIndex; //リンクボーンのボーンIndex
				unsigned char limitAngleFlag; //角度制限 0:OFF 1:ON

				float3 lowerLimit = { 0,0,0 }; //下限 (x,y,z) -> ラジアン角
				float3 upperLimit = { 0,0,0 }; //上限 (x,y,z) -> ラジアン角
			};
			vector<IKLink>IKlinks;
		};
	}
	struct PMXBone
	{
		PMXBone(size_t BoneIndexSize);
		~PMXBone();
		PMXBone(PMXBone&& Tmp);
		string name_JP;
		string name_EN;

		float3 pos;
		unsigned char* boneIndex;	//親ボーンのボーンインデックス
		int transLayer; //変形階層

		short boneFlag; //ボーンフラグ(16bit) 各bit 0:OFF 1:ON

		list<Bone::PMXBoneInfo*>info;
	};

	namespace Morph
	{
		struct PMXMorphInfo
		{
		protected:
			PMXMorphInfo() {};
		};

		struct Vertex : public PMXMorphInfo
		{
			Vertex(size_t VertexIndexSize);
			~Vertex();
			Vertex(Vertex&& Tmp);
			unsigned char* vertexIndex; //頂点Index
			float3 offset; //座標オフセット量(x,y,z)
		};

		struct UV : public PMXMorphInfo
		{
			UV(size_t VertexIndexSize);
			~UV();
			UV(UV&& Tmp);
			unsigned char* vertexIndex; //頂点Index
			float4 offset; //UVオフセット量(x,y,z,w) // ※通常UVはz,wが不要項目になるがモーフとしてのデータ値は記録しておく
		};

		struct Bone : public PMXMorphInfo
		{
			Bone(size_t BoneIndexSize);
			~Bone();
			Bone(Bone&& Tmp);
			unsigned char* boneIndex;	//ボーンIndex
			float3 move; //移動量
			float4 quaternion; //回転量
		};

		struct Material : PMXMorphInfo
		{
			Material(size_t MaterialIndexSize);
			~Material();
			Material(Material&& Tmp);
			unsigned char* materialIndex;
			unsigned char offsetFormat;	//オフセット演算形式、０：乗算、１：加算
			float4 duffse;
			float3 specular;
			float specular_c;
			float3 ambient;
			float4 edgeColor;
			float edgeSize;
			float4 tex_c;
			float4 sphere_c;
			float4 toonTex_c;
		};

		struct Group : public PMXMorphInfo
		{
			Group(size_t MorphIndexSize);
			~Group();
			Group(Group&& Tmp);
			unsigned char* morphIndex;
			float rate;	//モーフ率：グループモーフのモーフ値 * モーフ率 = 対象モーフのモーフ値
		};
	}
	struct PMXMorph
	{
		static const enum TYPE
		{
			GROUP,VERTEX,BONE,UV,
			ADDITIONAL_UV_1, ADDITIONAL_UV_2, ADDITIONAL_UV_3, ADDITIONAL_UV_4,
			MATERIAL,
		};
		string name_JP;
		string name_EN;
		unsigned char controlPanel; // 操作パネル (PMD:カテゴリ) 1:眉(左下) 2:目(左上) 3:口(右上) 4:その他(右下)  | 0:システム予約
		unsigned char type; //モーフ種類 - 0:グループ, 1:頂点, 2:ボーン, 3:UV, 4:追加UV1, 5:追加UV2, 6:追加UV3, 7:追加UV4, 8:材質
		unsigned int  offsetNum;
		list<Morph::PMXMorphInfo>data; //※異なる種類の混合は不可
	};
	//struct PMXDisplayBorder //表示枠
	//{
	//	string name_JP;
	//	string name_EN;
	//	unsigned char specialFlag; // 特殊枠フラグ - 0:通常枠 1:特殊枠
	//	int num; //枠内要素数 : 後続の要素数
	//	struct Display //枠内要素
	//	{
	//		unsigned char target; // 要素対象 0:ボーン 1:モーフ
	//		unsigned int index;
	//	};
	//	vector<Display>displays;
	//};

	struct PMXRigidBody
	{
		PMXRigidBody(size_t BoneIndexSize);
		~PMXRigidBody();
		PMXRigidBody(PMXRigidBody&& Tmp);
		string name_JP;
		string name_EN;

		unsigned char* boneIndex;

		unsigned char group;
		unsigned short notCollisionGroupFlag; //非衝突グループフラグ

		unsigned char shape; //形状 - 0:球 1:箱 2:カプセル
		float3 size;

		float3 pos;
		float3 rotate;

		float mass;	//質量
		float moveAttenuation;	//移動減衰
		float rotateAttenuation;	//回転減衰
		float rebound; //反発力
		float friction;	//摩擦力

		unsigned char physicalOperation; //剛体の物理演算 - 0:ボーン追従(static) 1:物理演算(dynamic) 2:物理演算 + Bone位置合わせ
	};

	struct PMXJoint
	{
		string name_JP;
		string name_EN;

		unsigned char type; //Joint種類 - 0:スプリング6DOF   | PMX2.0では 0 のみ(拡張用)

		struct JointInfo
		{
			JointInfo(size_t RigidBodyIndexSize);
			~JointInfo();
			JointInfo(JointInfo&& Tmp);
			unsigned char* rigidBodyIndexA; //関連剛体AのIndex - 関連なしの場合は-1
			unsigned char* rigidBodyIndexB; //関連剛体BのIndex - 関連なしの場合は-1
			float3 pos;
			float3 spin;
			float3 moveLimit_Lower;
			float3 moveLimit_Upper;
			float3 spinLimit_Lower;
			float3 spinLimit_Upper;
			float3 springConstant_Move;	//バネ定数-移動
			float3 springConstant_Spin;	//バネ定数-回転
		};
		JointInfo* info = nullptr;

		~PMXJoint();
	};

	struct PMXModel
	{
		PMX::PMXHeader head;
		PMX::PMXInfo info;

		int vertexNum;	//頂点数
		vector<PMXVertex>vertices;

		int surfaceNum;
		vector<PMXSurface>surfaces;

		int texNum;
		vector<string>texPass;

		int materialNum;
		vector<PMXMaterial>materials;

		int boneNum;
		list<PMXBone>bones;

		int morphNum;
		list<PMX::PMXMorph>morphs;

		int rigidBodyNum;
		vector<PMXRigidBody>rigitBodies;

		int jointNum;
		list<PMXJoint>joints;
	};
}

static class PMXLoader
{
	static void ConvertPMX(string Dir, PMX::PMXModel* Pmx, Model_HSM* Hsm);
public:
	static void LoadPMX(string Dir, string ModelName, Model_HSM** Model);
};

