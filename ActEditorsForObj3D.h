#pragma once
#include"ActivityEditor3D.h"
#include"Model_HSM.h"

//�A�N�e�B�r�e�B�G�f�B�^���ꊇ�Ǘ����邽�߂̒���N���X3D(Object3D�p)
class ActEditors_Obj3D
{
public:
	static const std::string EXTENSION;	//�g���q
private:
	friend void ImguiCommand_Master();
	ActivityEditor_3D* activityEditor[ACTIVE_TYPE_NUM] = { nullptr };
	std::string name;

	//���s������false��Ԃ�
	bool Load(std::string& ActEditorName, std::string* DirPass = nullptr);

public:
	ActEditors_Obj3D(std::string Name, XMFLOAT3* Pos, XMFLOAT3* Scale, XMFLOAT3* Rotation, bool Load = false, std::string* DirPass = nullptr);
	ActEditors_Obj3D(std::string Name, HSM::Transform& Transform, bool Load = false, std::string* DirPass = nullptr);
	~ActEditors_Obj3D();

	void Init();

	//�����l�Z�b�g
	void InitStatus();
	//�N��
	void AllOn();
	//���f
	void Interrupt();

	//.ae�t�@�C������ǂݍ���
	void LoadAE(std::string Name);

	//���݂̏�Ԃ�.ae�ɏo��
	void SaveAE(std::string* DirPass = nullptr);

	bool Finish();

	void SetTarget(XMFLOAT3* Pos, XMFLOAT3* Scale, XMFLOAT3* Rotation);
	void SetTarget(HSM::Transform& Transform);

	std::string GetName() { return name; }
};