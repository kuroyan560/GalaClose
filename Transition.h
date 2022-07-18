#pragma once
class Transition
{
protected:
	bool nowTrans = false;
public:
	virtual ~Transition() {};
	virtual void Start() { nowTrans = true; }	//シーン遷移スタート
	virtual bool Update() { return true; nowTrans = false; }	//シーンを切り替えるタイミングでtrueを返す
	virtual void Draw() {};
	bool Finish() { return !nowTrans; }	//シーン遷移が完全に終了したか
};