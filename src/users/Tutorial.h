#pragma once
#include<memory>
#include"../../Singleton.h"
#include<vector>
class Sprite_Normal;
class Tutorial : public Singleton<Tutorial>
{
	friend class Singleton <Tutorial>;
	Tutorial();

	//チュートリアル
	std::shared_ptr<Sprite_Normal>tutorialSprite;

	bool hide;
	int timer;
	float offsetX;

	//PushPower
	std::shared_ptr<Sprite_Normal>pushPowerSprite;
	std::vector<std::shared_ptr<Sprite_Normal>>powerSprite;
	std::shared_ptr<Sprite_Normal>powerBackSprite;
	int oldPower = 0;

public:
	void Init(const bool& Hide);
	void Update();
	void Draw();

	void Move();
};

