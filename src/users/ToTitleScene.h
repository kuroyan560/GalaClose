#pragma once
#include"../../Transition.h"
#include<memory>

class Sprite_Normal;

class ToTitleScene : public Transition
{
	std::shared_ptr<Sprite_Normal>pinkScreen;
	int timer;
	static const enum { APPEAR, WAIT, DISAPPEAR };
	int flag;

public:
	ToTitleScene();
	void Start()override;	//シーン遷移スタート
	bool Update()override;	//シーンを切り替えるタイミングでtrueを返す
	void Draw()override;
};

