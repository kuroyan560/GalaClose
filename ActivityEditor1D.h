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

	//ベジエのポイント
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
	static const std::string EXTENSION;	//拡張子
	static void ActivesInit();
	static void ActivesUpdate();

private:
	friend class ActEditors_Sprite;

	int frame;
	float* target;	//モーションの適応対象の変数
	std::vector<Active_1D>actives;
	int now;

	//ループさせるか
	bool loop;

	//開始時から作動させるか
	bool initStart;
	//アクティビティエディタのスイッチ（オフのときは作用しない）
	bool onFlag;

	//中断している間フレーム数を相殺するための変数
	int offsetFrame;

	void Init();
	void Update();
	void CreateActive(int NowSelectMode = -1);
	void DeleteActive(int Num);

public:
	ActivityEditor_1D(float* Target) :target(Target) { staticActives.push_back(this); }
	~ActivityEditor_1D() { remove(staticActives.begin(), staticActives.end(), this); }

	bool Save(std::string& TextStr);

	//引数なしのとき、初期位置にセットされる
	bool UpdateFromFrame(int ActiveIndex = 0, int Frame = 0);

	void SetTarget(float* Target) { target = Target; }

	//起動
	void On();

	//中断
	void Interrupt();

	//リセット(ターゲットはそのままに)
	void ResetActives() { actives.clear(); }
};