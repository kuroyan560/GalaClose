#pragma once
#include<DirectXMath.h>
using namespace DirectX;

#include<vector>

#include<wrl.h>
#include<d3d12.h>

#include"DirtyFlag.h"
#include<string>

#include"Master.h"
#include<map>

#include"ActEditorsForObj3D.h"
#include"Model_HSM.h"

class Object3D
{
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	friend class Master;

	//オブジェクトの名前と個数
	static std::map<std::string, int>registeredObjInfo;

public:
	static std::vector<Object3D*> objs;
	static void MatConstMappingAll();
	static void InitAll();
	static void FinalizeAll();

	static void RegisterObj(Object3D* Obj) 
	{
		Obj->objID = objs.size();
		objs.push_back(Obj); 
	}
	static void StaticInit() { objs.clear(); }

protected:
	friend void ImguiCommand_Master();
	struct Const
	{
		XMMATRIX viewMat;
		XMMATRIX projMat;    // ビュープロジェクション行列
		XMMATRIX worldMat; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
	};

	XMMATRIX matWorld;

	ComPtr<ID3D12Resource> constBuff;

	bool isBillboard = false;
	bool isBillboardY = false;

	DirtyFlag matDirty;

	Object3D(string Name, XMFLOAT3& Pos, XMFLOAT3& Scale, XMFLOAT3& Rotation);
	virtual ~Object3D();

	virtual void MatConstMapping();
	void MatConstSet(int RootParamIndex);

	XMFLOAT3 initPos = { 0,0,0 };
	XMFLOAT3 initScale = { 1,1,1 };
	XMFLOAT3 initRotation = { 0,0,0 };
	virtual void UniqueInit() = 0;	//座標スケール回転以外でデバッグモードでいじりたいものはこの中で（※何も無いときは空欄）
	virtual void UniqueUpdate() = 0;
	virtual void UniqueFinalize() = 0;		//同上

	//アクション統括
	ActEditors_Obj3D* actEditors = nullptr;

	int objID;

public:
	string name;
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 up = { 0,1,0 };
	bool isAlive = true;
	XMMATRIX* parentMat = nullptr;

	const XMMATRIX& GetMatWorld() { return matWorld; }
	bool Dirty() { return matDirty.Dirty(); }

	void Init();	//デバッグモードの終了、ゲームモードの開始
	void Update();
	virtual void Draw() = 0;
	void Finalize();		//ゲームモードの終了、デバッグモードの開始

	int GetObjID()const { return objID; }

	DirtyFlag& GetDirtyFlag() { return matDirty; }

	////当たり判定インターフェイスオーバーライド
	//virtual bool HitCheckFlag(BaseCollider* Target)override { return true; }
	//virtual void OnCollision(const CollisionInfo& info)override {}

	//アクティブエディタの生成
	void CreateActiveEditor(std::string EditorName);
	//アクティブエディタの削除
	void DeleteActiveEditor();
	//失敗したらfalseを返す
	bool LoadActive(std::string ActiveName, bool Save = false);
	void SetActive(ActEditors_Obj3D* Ptr);
};