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

	//ベジエのポイント
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
	static const std::string EXTENSION;	//拡張子
	static void ActivesInit();
	static void ActivesUpdate();

private:
	friend class ActEditors_Sprite;
	friend class ActEditorsForCam;

	int frame;
	XMFLOAT2* target;	//モーションの適応対象の変数
	std::vector<Active_2D>actives;
	int now;

	//ループさせるか
	bool loop;

	//開始時から作動させるか
	bool initStart;
	//アクティビティエディタのスイッチ（オフのときは作用しない）
	bool onFlag;

	//中断している間フレーム数を相殺するための変数
	int offsetFrame;

	//直接代入ではなく加算
	bool addFlag = false;

	void Init();
	void Update();
	void CreateActive(int NowSelectMode);
	void DeleteActive(int Num);

public:
	ActivityEditor_2D(XMFLOAT2* Target) :target(Target) { staticActives.push_back(this); }
	~ActivityEditor_2D() { remove(staticActives.begin(), staticActives.end(), this); }

	bool Save(std::string& TextStr);

	//引数なしのとき、初期位置にセットされる
	bool UpdateFromFrame(int ActiveIndex, int Frame);
	void UpdateFromFrame(int Frame);

	void SetTarget(XMFLOAT2* Target) { target = Target; }

	//起動
	void On();

	//中断
	void Interrupt();

	//リセット(ターゲットはそのままに)
	void ResetActives() { actives.clear(); }

	//動きを終了しているか
	bool Finish();
};