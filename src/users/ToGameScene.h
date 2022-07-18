#pragma once
#include"../../Transition.h"
#include<memory>

class Sprite_Normal;

class ToGameScene : public Transition
{
	std::shared_ptr<Sprite_Normal>ready;
	int timer;

public:
	ToGameScene();
	virtual void Start()override;	//シーン遷移スタート
	virtual bool Update()override;	//シーンを切り替えるタイミングでtrueを返す
	virtual void Draw()override;
};