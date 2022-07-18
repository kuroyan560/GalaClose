#pragma once
#include<vector>
#include "AnimationInfo.h"
#include"Model_HSM.h"

using namespace AnimationInfo;

class Action
{
	static std::vector<Action*>staticActions;
	static const std::string EXTENTION;
	static void ConvertToAnimFile(std::string& FileName, Action* Action);
	static void LoadAnimFile(std::string& FileName, Action* Action);
public:
	static void AllActionUpdate();
	static void AllActionInit();
	static void LoadAction(std::string Dir, std::string FileName, Action** ActionTarget, AFFECT_MODE AffectMode, HSM::Skeleton* Skeleton);
private:
	friend class FBXLoader;
	
	//�t�@�C����
	std::string name;
	std::string dir;

	HSM::BoneGroup* target = nullptr;
	//�{�[���̖��O�A�A�j���[�V����
	std::vector<AnimationInfo::Animation>animations;
	bool isPlay = false;	//�A�j���[�V��������
	int past = 0;			//�o�߃t���[��
	int total = 0;			//�g�[�^���t���[��
	AFFECT_MODE affectMode;
	bool loop = false;

	//�|�[�Y
	HSM::BoneGroup* poseTarget = nullptr;
	int poseKeyFrame;

	//void GetValue(float& t, float3& Target, VALUE_MODE ValueMode, std::vector<AnimationInfo::Animation>::iterator& AnimationItr);
	//void GetValue(int& PastFrame, float3& Target, VALUE_MODE ValueMode, std::vector<AnimationInfo::Animation>::iterator& AnimationItr);
	XMMATRIX GetKeyValue(int Past, AnimationInfo::Animation Animation);

	void Update();

public:
	Action(AFFECT_MODE AffectMode);
	void Init();
	void Play(HSM::BoneGroup* BoneGroup, bool Loop, bool NowUpdate = false);
	bool IsPlay() { return isPlay; }
	void Stop();
	void Pose(int KeyFrame, HSM::BoneGroup* BoneGroup);
};

