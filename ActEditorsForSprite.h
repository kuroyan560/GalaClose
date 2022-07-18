#pragma once
#include"ActivityEditor2D.h"
#include"ActivityEditor1D.h"
#include<string>

//�A�N�e�B�r�e�B�G�f�B�^���ꊇ�Ǘ����邽�߂̒���N���X(Sprite�p)
class ActEditors_Sprite
{
public:
	static const std::string EXTENSION;	//�g���q

private:
	friend void ImguiCommand_Master();
	ActivityEditor_2D* actEditor_pos = nullptr;
	ActivityEditor_2D* actEditor_scale = nullptr;
	ActivityEditor_1D* actEditor_rotate = nullptr;
	std::string name;

	//���s������false��Ԃ�
	bool Load(std::string& ActEditorName);

public:
	ActEditors_Sprite(std::string Name, XMFLOAT2* Pos, XMFLOAT2* Scale, float* Rotation);
	~ActEditors_Sprite();

	//�����l�Z�b�g
	void InitStatus();
	//�N��
	void AllOn();
	//���f
	void Interrupt();

	ActivityEditor_2D* GetEditor2D(int EditorType)
	{
		if (EditorType == ACTIVE_POS)return actEditor_pos;
		else if (EditorType == ACTIVE_SCALE)return actEditor_scale;
		else return false;
	}

	//.ae�t�@�C������ǂݍ���
	void LoadAE(std::string Name);

	//���݂̏�Ԃ�.ae�ɏo��
	void SaveAE();

	//���O�̃Q�b�^
	std::string GetName() { return name; }
};

