#pragma once
#include"ActivityEditor.h"
#include<DirectXMath.h>
using namespace DirectX;

#include<vector>
#include<string>

struct Active_3D
{
	int frame = 60;
	XMFLOAT3 start = { 0,0,0 };
	XMFLOAT3 end = { 0,0,0 };

	//�x�W�G�̃|�C���g
	XMFLOAT2 p1 = { 0,0 };
	XMFLOAT2 p2 = { 1,1 };

	Active_3D() {};
	Active_3D(XMFLOAT3* Target, int NowSelectMode)
	{
		start = end = *Target;
		if (NowSelectMode == ACTIVE_SCALE)
		{
			start = { 1,1,1 };
			end = { 1,1,1 };
		}
	}
	Active_3D(Active_3D* PreActive)
		:start(PreActive->end), end(PreActive->end), frame(PreActive->frame),
		p1(PreActive->p1), p2(PreActive->p2) {};
};

class ActivityEditor_3D
{
private:
	static std::vector<ActivityEditor_3D*> staticActives;

	friend void ImguiCommand_Master();
	friend void ImguiCommand_StageMgr();
	friend void ImguiCommand_Camera();

public:
	static void ActivesInit();
	static void ActivesUpdate();

private:
	friend class ActEditors_Obj3D;
	friend class ActEditorsForCam;

	XMFLOAT3* target;	//���[�V�����̓K���Ώۂ̕ϐ�
	std::vector<Active_3D>actives;
	int now;
	int frame;

	//���[�v�����邩
	bool loop;

	//�J�n������쓮�����邩
	bool initStart;
	//�A�N�e�B�r�e�B�G�f�B�^�̃X�C�b�`�i�I�t�̂Ƃ��͍�p���Ȃ��j
	bool onFlag;

	//���f�t���O
	int offsetFrame;

	//���ڑ���ł͂Ȃ����Z
	bool addFlag = false;

	void Update();
	void CreateActive(int NowSelectMode);
	void DeleteActive(int Num);

public:
	ActivityEditor_3D(XMFLOAT3* Target);
	~ActivityEditor_3D();

	void Init();

	bool Save(std::string& TextStr);

	bool UpdateFromFrame(int ActiveIndex, int Frame);
	void UpdateFromFrame(int Frame);

	void SetTarget(XMFLOAT3* Target) { target = Target; }

	//�N��
	void On();

	//���f
	void Interrupt();

	//���Z�b�g(�^�[�Q�b�g�͂��̂܂܂�)
	void ResetActives() { actives.clear(); }

	//�������I�����Ă��邩
	bool Finish();
};