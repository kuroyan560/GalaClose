#pragma once
#include"Layer.h"
#include<vector>
#include"Constants.h"

class Panel
{
	static int PUSH_SE;
	static int PUSH_RESET_SE;
public:
	static void LoadAudios();

private:
	float pushedRate = 0.0f;		//�p�l���������݂̊���
	std::vector<Layer>layers;
	float startAngle;
	float endAngle;

	int pushedTimer;	//�������݂̎��Ԃ𑪂�^�C�}�[
	int oldPushedLayer = 0;	//�ω��O�̃��C���[�̐�

	int resetTimer = 0;		//�ő�܂ŉ������܂�Ă��烊�Z�b�g����܂ł̎��Ԃ𑪂�^�C�}�[
	const int panelIndex;

	bool pushed = false;
	int offsetLayerNumByAddLayer = 0;
	int pushBuff = 0;
	int pushBuffSubTimer = 0;

	void OneLayerPush();

public:
	int pushedLayer = 0;	//�������܂ꂽ���C���[�̐�

	Panel(const float& StartAngle, const float& EndAngle, const int& LayerNum, const int& PanelIndex, const bool& ColorDifference);
	void Update();
	void Draw();

	//void Pushed(const int& LayerNum);
	void Pushed(const int& LayerNum);
	void PushReset();
	void AddLayer();

	//�������܂�Ă���
	bool NowPushed() { return pushed || pushBuff; }
	//�K�w�������Ă���i���₷�ۂɊ��ɉ������܂�Ă����ꍇ�̊K�w�I�t�Z�b�g�l�j
	int GetOffsetLayerNumByAddLayer() { return offsetLayerNumByAddLayer; }
	//�������܂ꂽ�u��
	int PushedTriggerRelative () {
		if (pushed && pushedTimer == PUSH_TOTAL_TIME - 1)return pushedLayer - oldPushedLayer;	//����
		else return 0;
	}
	bool PushedTrigger() { return pushedTimer == 1; }
	//�K�w���������u��
	bool AddLayerTrigger() { return offsetLayerNumByAddLayer && pushedTimer == PUSH_TOTAL_TIME - 1; }

	//�����蔻�莞�A�G�����̃p�l���ɓ��������i��������������Ȃ��Ɣ��]�j
	bool CanEnter() {	return !(NowPushed() || GetOffsetLayerNumByAddLayer() || resetTimer != 0);	}

	//�p�l������\��
	bool CanControl() { return !NowPushed(); }

	//�����O�̏�Ԃ̃��C���[��ۑ����邱��
	void RegisterLayerBeforeExplosion();

	//�Q�b�^
	const int& GetPanelIndex() { return panelIndex; }
	const float& GetStartAngle() { return startAngle; }
	const float& GetEndAngle() { return endAngle; }
	const char& GetLayerColor(const int& LayerIndex);
	const float& GetPushedRate() { return pushedRate - oldPushedLayer; }
};