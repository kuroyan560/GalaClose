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
	float pushedRate = 0.0f;		//パネル押し込みの割合
	std::vector<Layer>layers;
	float startAngle;
	float endAngle;

	int pushedTimer;	//押し込みの時間を測るタイマー
	int oldPushedLayer = 0;	//変化前のレイヤーの数

	int resetTimer = 0;		//最大まで押し込まれてからリセットするまでの時間を測るタイマー
	const int panelIndex;

	bool pushed = false;
	int offsetLayerNumByAddLayer = 0;
	int pushBuff = 0;
	int pushBuffSubTimer = 0;

	void OneLayerPush();

public:
	int pushedLayer = 0;	//押し込まれたレイヤーの数

	Panel(const float& StartAngle, const float& EndAngle, const int& LayerNum, const int& PanelIndex, const bool& ColorDifference);
	void Update();
	void Draw();

	//void Pushed(const int& LayerNum);
	void Pushed(const int& LayerNum);
	void PushReset();
	void AddLayer();

	//押し込まれている
	bool NowPushed() { return pushed || pushBuff; }
	//階層が増えている（増やす際に既に押し込まれていた場合の階層オフセット値）
	int GetOffsetLayerNumByAddLayer() { return offsetLayerNumByAddLayer; }
	//押し込まれた瞬間
	int PushedTriggerRelative () {
		if (pushed && pushedTimer == PUSH_TOTAL_TIME - 1)return pushedLayer - oldPushedLayer;	//相対
		else return 0;
	}
	bool PushedTrigger() { return pushedTimer == 1; }
	//階層が増えた瞬間
	bool AddLayerTrigger() { return offsetLayerNumByAddLayer && pushedTimer == PUSH_TOTAL_TIME - 1; }

	//当たり判定時、敵がそのパネルに入れる条件（条件が満たされないと反転）
	bool CanEnter() {	return !(NowPushed() || GetOffsetLayerNumByAddLayer() || resetTimer != 0);	}

	//パネル操作可能か
	bool CanControl() { return !NowPushed(); }

	//爆発前の状態のレイヤーを保存すること
	void RegisterLayerBeforeExplosion();

	//ゲッタ
	const int& GetPanelIndex() { return panelIndex; }
	const float& GetStartAngle() { return startAngle; }
	const float& GetEndAngle() { return endAngle; }
	const char& GetLayerColor(const int& LayerIndex);
	const float& GetPushedRate() { return pushedRate - oldPushedLayer; }
};