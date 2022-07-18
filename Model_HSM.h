#pragma once
#include"Material_HSMA.h"
#include<list>
#include"Camera.h"

#include"HSM.h"

class Individualinfo_ForIncrease;

//���f�����A���_�f�[�^�A�C���f�b�N�X�f�[�^�ێ�
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
	HSM::Header head; //���f�����w�b�_

	//���_�f�[�^
	vector<HSM::Vertex> vertices;
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	//�C���f�b�N�X�f�[�^
	vector<unsigned int>indices;
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	//�}�e���A���f�[�^		//���v�f���ꏏ
	vector<Material_HSMA>materials;
	//�}�e���A���C���f�b�N�X�A���_��
	vector<HSM::Mesh> meshes;

	//�{�[���f�[�^
	HSM::Skeleton skeleton;

	bool culling;

	Model_HSM(bool Culling = true);
	//GPU���\�[�X����
	void CreateBuff();

	//�擾�����{�[����񂩂�c���[���\�z
	void CreateBoneTree();

	//���_�ƃC���f�b�N�X�A�}�e���A���ȂǊ�{�I�Ȃ��̂��Z�b�g����DrawInstance���s��
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
	//�g�����X�t�H�[��(���W�A�g�k�A��])
	HSM::Transform transform;
	//��\���ɂ��������b�V�������X�g
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

	void DirtyMapping();	//�_�[�e�B�t���O��true�̂Ƃ������}�b�s���O����
	bool TransformDirty() { return transformDirty.Dirty(); }
};


//�̏��
struct Individualinfo_ForIncrease
{
	Model_HSM_Component component;
	HSM::BoneGroup boneGroup;	//�{�[���Ɋ֘A������
	Individualinfo_ForIncrease(HSM::Skeleton* ModelSkeleton)
		:boneGroup(ModelSkeleton) {};
	//vector<XMMATRIX>boneMatrices;	//�{�[���s��
	//void Init(HSM::Skeleton* ModelBoneOperator);
	//void TransformAffectToMat();
};

//�P�̃C���X�^���X�ŉ�����`�悷�邽�߂̋@�\
class Model_HSM_ForIncrease : public Model_HSM
{
public:
	void PreDraw();
	void DrawByComponent(Model_HSM_Component& Component);	//PreDraw�̌�
	void ManyDraw(std::list<Model_HSM_Component*>* Component);
	void ManyDraw(std::list<Individualinfo_ForIncrease*>* Info);
};

//��{�I�ȋ@�\��������N���X
class Model_HSM_Normal
{
protected:
	Model_HSM* model = nullptr;
	Model_HSM_Component component;
	HSM::BoneGroup boneGroup;	//�{�[���Ɋ֘A������
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

//��ɑO�ɕ\������
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