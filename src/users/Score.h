#pragma once
#include"../../Singleton.h"
#include<memory>
#include<vector>

class Sprite_Divide;
class Sprite_Normal;

class Score : public Singleton<Score>
{
	friend class Singleton<Score>;
	Score();

	std::vector<std::shared_ptr<Sprite_Divide>>numSprite;
	int score;
	int drawScore;
	int ko;
	//int addScore = 0;

	int scoreRateAddCount = 0;

	//std::shared_ptr<Sprite_Normal>comboSprite;
	std::vector<std::shared_ptr<Sprite_Divide>>addNumSprite;
	float addScoreX;
	int addScoreTimer;
	int waitTimer;

public:
	void Init();
	void Update();
	void Draw();

	void Add();
	void ComboReset() { ko = 0; }
	int* GetScorePtr() { return &score; }
};
