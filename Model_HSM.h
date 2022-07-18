#pragma once
#include"Material_HSMA.h"
#include<list>
#include"Camera.h"

#include"HSM.h"

class Individualinfo_ForIncrease;

//モデル情報、頂点データ、インデックスデータ保持
class Model_HSM
{
protected:
	friend class PMXLoader;
	friend class FBXLoader;
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const string PIPELINE_KEY;
	static const string PIPELINE_KEY_NON_CULLING;

public:
	static void CreatePipeline();

protected:
	friend class Model_HSM_Normal;
	friend class Model_HSM_Front;
	HSM::Header head; //モデル情報ヘッダ

	//頂点データ
	vector<HSM::Vertex> vertices;
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	//インデックスデータ
	vector<unsigned int>indices;
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	//マテリアルデータ		//→要素数一緒
	vector<Material_HSMA>materials;
	//マテリアルインデックス、頂点数
	vector<HSM::Mesh> meshes;

	//ボーンデータ
	HSM::Skeleton skeleton;

	bool culling;

	Model_HSM(bool Culling = true);
	//GPUリソース生成
	void CreateBuff();

	//取得したボーン情報からツリーを構築
	void CreateBoneTree();

	//頂点とインデックス、マテリアルなど基本的なものをセットしてDrawInstanceを行う
	void DrawIndexedInstanced(HSM::BoneGroup* BoneGroup);

	void DrawIndexedInstancedIndividual(Individualinfo_ForIncrease& Info);

public:
	HSM::Skeleton* GetSkeleton() { return &skeleton; }
	vector<HSM::Mesh>& GetMeshes() { return meshes; }
	float3 GetVertexPos(int VertexIndex) { return vertices[VertexIndex].pos; }
	int GetIndex(int Index) { return indices[Index]; }
};

class Model_HSM_Component
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> colorBuff = nullptr;
	DirtyFlag masterColorDirty;

	ComPtr<ID3D12Resource> transformBuff = nullptr;
	DirtyFlag transformDirty;

public:
	HSM::MasterColor masterColor;
	//トランスフォーム(座標、拡縮、回転)
	HSM::Transform transform;
	//非表示にしたいメッシュ名リスト
	std::list<string>nonDrawMaterialName;

	Model_HSM_Component();
	~Model_HSM_Component();

	void DirtyRegister();
	void DirtyRemove();
	void DirtyRecord();

	void TransformMatUpdate();

	void TransformMapping();
	void MasterColorMapping();
	void SetComponentConstBuff();

	void DirtyMapping();	//ダーティフラグがtrueのときだけマッピングする
	bool TransformDirty() { return transformDirty.Dirty(); }
};


//個体情報
struct Individualinfo_ForIncrease
{
	Model_HSM_Component component;
	HSM::BoneGroup boneGroup;	//ボーンに関連する情報
	Individualinfo_ForIncrease(HSM::Skeleton* ModelSkeleton)
		:boneGroup(ModelSkeleton) {};
	//vector<XMMATRIX>boneMatrices;	//ボーン行列
	//void Init(HSM::Skeleton* ModelBoneOperator);
	//void TransformAffectToMat();
};

//１つのインスタンスで何回も描画するための機能
class Model_HSM_ForIncrease : public Model_HSM
{
public:
	void PreDraw();
	void DrawByComponent(Model_HSM_Component& Component);	//PreDrawの後
	void ManyDraw(std::list<Model_HSM_Component*>* Component);
	void ManyDraw(std::list<Individualinfo_ForIncrease*>* Info);
};

//基本的な機能を備えたクラス
class Model_HSM_Normal
{
protected:
	Model_HSM* model = nullptr;
	Model_HSM_Component component;
	HSM::BoneGroup boneGroup;	//ボーンに関連する情報
	virtual void SetPipeline();
	virtual void SetCamConstBuff();
	virtual void SetAdditionalBuff() {};

public:
	Model_HSM_Normal(Model_HSM* Model);
	~Model_HSM_Normal();
	Model_HSM_Component& GetComponent() { return component; }
	HSM::Transform& GetTransform() { return component.transform; }
	HSM::BoneGroup& GetBoneGroup() { return boneGroup; }
	Model_HSM* GetModel() { return model; }
	void Init();
	void Draw();
};

//常に前に表示する
class Model_HSM_Front :public Model_HSM_Normal
{
	static const string PIPELINE_KEY;
	static const string PIPELINE_KEY_NON_CULLING;
protected:
	NonMoveCam CAM;
	virtual void SetPipeline()override;
	void SetCamConstBuff()override;
public:
	static void CreatePipeline();
	Model_HSM_Front(Model_HSM* Model) :Model_HSM_Normal(Model) {}
};