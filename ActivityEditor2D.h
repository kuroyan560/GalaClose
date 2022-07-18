#pragma once
#include"ActivityEditor.h"
#include<DirectXMath.h>
using namespace DirectX;

#include<vector>
#include<string>

struct Active_2D
{
	int frame = 60;
	XMFLOAT2 start = { 0,0 };
	XMFLOAT2 end = { 0,0 };

	//�x�W�G�̃|�C���g
	XMFLOAT2 p1 = { 0,0 };
	XMFLOAT2 p2 = { 1,1 };

	Active_2D() {};
	Active_2D(XMFLOAT2* Target, int NowSelectMode)
	{
		start = end = *Target;
		if (NowSelectMode == ACTIVE_SCALE)
		{
			start = { 1,1 };
			end = { 1,1 };
		}
	}
	Active_2D(XMFLOAT2& Start, int& Frame) :start(Start), end(Start), frame(Frame) {};
};

class ActivityEditor_2D
{
private:
	static std::vector<ActivityEditor_2D*> staticActives;

	friend void ImguiCommand_Master();
	friend void ImguiCommand_Camera();

public:
	static const std::string EXTENSION;	//�g���q
	static void ActivesInit();
	static void ActivesUpdate();

private:
	friend class ActEditors_Sprite;
	friend class ActEditorsForCam;

	int frame;
	XMFLOAT2* target;	//���[�V�����̓K���Ώۂ̕ϐ�
	std::vector<Active_2D>actives;
	int now;

	//���[�v�����邩
	bool loop;

	//�J�n������쓮�����邩
	bool initStart;
	//�A�N�e�B�r�e�B�G�f�B�^�̃X�C�b�`�i�I�t�̂Ƃ��͍�p���Ȃ��j
	bool onFlag;

	//���f���Ă���ԃt���[�����𑊎E���邽�߂̕ϐ�
	int offsetFrame;

	//���ڑ���ł͂Ȃ����Z
	bool addFlag = false;

	void Init();
	void Update();
	void CreateActive(int NowSelectMode);
	void DeleteActive(int Num);

public:
	ActivityEditor_2D(XMFLOAT2* Target) :target(Target) { staticActives.push_back(this); }
	~ActivityEditor_2D() { remove(staticActives.begin(), staticActives.end(), this); }

	bool Save(std::string& TextStr);

	//�����Ȃ��̂Ƃ��A�����ʒu�ɃZ�b�g�����
	bool UpdateFromFrame(int ActiveIndex, int Frame);
	void UpdateFromFrame(int Frame);

	void SetTarget(XMFLOAT2* Target) { target = Target; }

	//�N��
	void On();

	//���f
	void Interrupt();

	//���Z�b�g(�^�[�Q�b�g�͂��̂܂܂�)
	void ResetActives() { actives.clear(); }

	//�������I�����Ă��邩
	bool Finish();
};