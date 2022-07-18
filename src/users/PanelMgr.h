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
	int maxLayerNum;	//現在の最大レイヤー数
	int oldMaxLayerNum;
	float panelAngleOffset = 0.0f;

	//スコアアタックモード
	bool scoreAtack;
	//パネル数変化演出
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
	void PushLayer(const int& PanelIndex, const int& LayerNum);	//パネルを選択して押し込む
	void ResetLayer(const int& PanelIndex);	//パネルを選択して押し込みリセット
	void AllResetLayer();

	const int& GetMaxLayerNum() { return maxLayerNum; }

	//階層が増えた
	bool AddMaxLayerTrigger() { return oldMaxLayerNum != maxLayerNum; }

	//引数が０より小さいときは最大インデックス、最大インデックスより大きいときは０に補正される
	std::shared_ptr<Panel> GetPanelByIndex(int PanelIndex);

	//引数に取った角度がどのパネルの範囲内か取得
	std::shared_ptr<Panel>GetPanelByAngle(float Angle, bool IsClockWise);

	//隣り合うパネルのオフセットアングル
	const float& GetPanelAngleOffset() { return panelAngleOffset; }

	//スコアアタック用、パネル数増加、レイヤーリセット
	//MAXになったら1を返す
	int AddPanelAndLayerReset();
};