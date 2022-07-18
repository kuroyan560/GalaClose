#pragma once
#include"ActivityEditor.h"
#include<DirectXMath.h>
using namespace DirectX;

#include<vector>
#include<string>

struct Active_1D
{
	int frame = 60;
	float start = 0;
	float end = 0;

	//�x�W�G�̃|�C���g
	XMFLOAT2 p1 = { 0,0 };
	XMFLOAT2 p2 = { 1,1 };

	Active_1D() {};
	Active_1D(float* Target, int NowSelectMode)
	{
		start = end = *Target;
		if (NowSelectMode == ACTIVE_SCALE)
		{
			start = 1;
			end = 1;
		}
	}
	Active_1D(float& Start, int& Frame) :start(Start), end(Start), frame(Frame) {};
};

class ActivityEditor_1D
{
private:
	static std::vector<ActivityEditor_1D*> staticActives;

	friend void ImguiCommand_Master();

public:
	static const std::string EXTENSION;	//�g���q
	static void ActivesInit();
	static void ActivesUpdate();

private:
	friend class ActEditors_Sprite;

	int frame;
	float* target;	//���[�V�����̓K���Ώۂ̕ϐ�
	std::vector<Active_1D>actives;
	int now;

	//���[�v�����邩
	bool loop;

	//�J�n������쓮�����邩
	bool initStart;
	//�A�N�e�B�r�e�B�G�f�B�^�̃X�C�b�`�i�I�t�̂Ƃ��͍�p���Ȃ��j
	bool onFlag;

	//���f���Ă���ԃt���[�����𑊎E���邽�߂̕ϐ�
	int offsetFrame;

	void Init();
	void Update();
	void CreateActive(int NowSelectMode = -1);
	void DeleteActive(int Num);

public:
	ActivityEditor_1D(float* Target) :target(Target) { staticActives.push_back(this); }
	~ActivityEditor_1D() { remove(staticActives.begin(), staticActives.end(), this); }

	bool Save(std::string& TextStr);

	//�����Ȃ��̂Ƃ��A�����ʒu�ɃZ�b�g�����
	bool UpdateFromFrame(int ActiveIndex = 0, int Frame = 0);

	void SetTarget(float* Target) { target = Target; }

	//�N��
	void On();

	//���f
	void Interrupt();

	//���Z�b�g(�^�[�Q�b�g�͂��̂܂܂�)
	void ResetActives() { actives.clear(); }
};