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

	//�I�u�W�F�N�g�̖��O�ƌ�
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
		XMMATRIX projMat;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX worldMat; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
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
	virtual void UniqueInit() = 0;	//���W�X�P�[����]�ȊO�Ńf�o�b�O���[�h�ł����肽�����̂͂��̒��Łi�����������Ƃ��͋󗓁j
	virtual void UniqueUpdate() = 0;
	virtual void UniqueFinalize() = 0;		//����

	//�A�N�V��������
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

	void Init();	//�f�o�b�O���[�h�̏I���A�Q�[�����[�h�̊J�n
	void Update();
	virtual void Draw() = 0;
	void Finalize();		//�Q�[�����[�h�̏I���A�f�o�b�O���[�h�̊J�n

	int GetObjID()const { return objID; }

	DirtyFlag& GetDirtyFlag() { return matDirty; }

	////�����蔻��C���^�[�t�F�C�X�I�[�o�[���C�h
	//virtual bool HitCheckFlag(BaseCollider* Target)override { return true; }
	//virtual void OnCollision(const CollisionInfo& info)override {}

	//�A�N�e�B�u�G�f�B�^�̐���
	void CreateActiveEditor(std::string EditorName);
	//�A�N�e�B�u�G�f�B�^�̍폜
	void DeleteActiveEditor();
	//���s������false��Ԃ�
	bool LoadActive(std::string ActiveName, bool Save = false);
	void SetActive(ActEditors_Obj3D* Ptr);
};