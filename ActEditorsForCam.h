#pragma once
#include"ActivityEditor3D.h"
#include"ActivityEditor2D.h"
class GameCamera;

class ActEditorsForCam
{
public:
	static const std::string EXTENSION;	//�g���q
	static const std::string DIRECTORY_PATH;
private:
	friend void ImguiCommand_Master();
	friend void ImguiCommand_StageMgr();
	friend void ImguiCommand_Camera();
	static const enum ACT_EDITOR_TYPE { EYE, TARGET };
	ActivityEditor_3D* actEditors[2] = { nullptr };
	std::string name;

	//���s������false��Ԃ�
	//bool Load(std::string& ActEditorName);

public:
	ActEditorsForCam(std::string Name, GameCamera* GameCam/*, bool Load = false*/);
	~ActEditorsForCam();

	void Init();
	void Update();

	//�����l�Z�b�g
	void InitStatus();
	//�N��
	void AllOn();
	//���f
	void Interrupt();

	////.ae�t�@�C������ǂݍ���
	//void LoadAE(std::string Name);

	////���݂̏�Ԃ�.ae�ɏo��
	//void SaveAE();

	bool Finish();

	void SetTarget(XMFLOAT3* Eye, XMFLOAT3* Target);

	std::string GetName() { return name; }

	//�����łǂꂭ�炢�̃t���[���������邩
	//int GetAllFrame();

	//��񂪑��݂��邩
	bool Empty();
};