#pragma once
#include"../../Singleton.h"
#include<vector>
#include<memory>
#include"Panel.h"

class Sprite_Normal;

class PanelMgr : public Singleton<PanelMgr>
{
	friend class Singleton<PanelMgr>;
	PanelMgr();

	std::vector<std::shared_ptr<Panel>>panels;
	int maxLayerNum;	//���݂̍ő僌�C���[��
	int oldMaxLayerNum;
	float panelAngleOffset = 0.0f;

	//�X�R�A�A�^�b�N���[�h
	bool scoreAtack;
	//�p�l�����ω����o
	static const int PANEL_CHANGE_EASE_TOTAL_TIME_0 = 60;
	static const int PANEL_CHANGE_EASE_TOTAL_TIME_1 = 90;
	int panelChangeFlag = -1;
	std::shared_ptr<Sprite_Normal>panelChangeSprite;
	int panelChangeTimer;

	void AddMaxLayer();

	int lvUpSE;

public:
	void Init(int PanelNum, int MaxLayerNum, bool ColorDifference, bool ScoreAtack = false);
	void Update();
	void Draw();
	void UIDraw();
	void Finalize();

	const int& GetPanelNum() { return panels.size(); }
	void PushLayer(const int& PanelIndex, const int& LayerNum);	//�p�l����I�����ĉ�������
	void ResetLayer(const int& PanelIndex);	//�p�l����I�����ĉ������݃��Z�b�g
	void AllResetLayer();

	const int& GetMaxLayerNum() { return maxLayerNum; }

	//�K�w��������
	bool AddMaxLayerTrigger() { return oldMaxLayerNum != maxLayerNum; }

	//�������O��菬�����Ƃ��͍ő�C���f�b�N�X�A�ő�C���f�b�N�X���傫���Ƃ��͂O�ɕ␳�����
	std::shared_ptr<Panel> GetPanelByIndex(int PanelIndex);

	//�����Ɏ�����p�x���ǂ̃p�l���͈͓̔����擾
	std::shared_ptr<Panel>GetPanelByAngle(float Angle, bool IsClockWise);

	//�ׂ荇���p�l���̃I�t�Z�b�g�A���O��
	const float& GetPanelAngleOffset() { return panelAngleOffset; }

	//�X�R�A�A�^�b�N�p�A�p�l���������A���C���[���Z�b�g
	//MAX�ɂȂ�����1��Ԃ�
	int AddPanelAndLayerReset();
};