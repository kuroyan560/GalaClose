#pragma once
#include <vector>
#include"../../Singleton.h"
#include<memory>

//コンストラクタで読み込んで
//IsLevelUpで内容処理、Resetでレベルの初期化
//他は使わない
class Sprite_Divide;
class Sprite_Normal;

class Object2D;
class Object2D_Divide;

class ScoreLevel : public Singleton<ScoreLevel>
{
	friend class Singleton<ScoreLevel>;
	ScoreLevel();

public:
	void Init(int* ScorePtr);
	void Update();
	void DrawObj();
	void DrawUI();
	void Finalize();

private://メンバ変数

	int lv = 0;
	bool lvMax = false;

	std::vector<std::vector<int>> borders;
	int nowBorder;
	int* score = nullptr;

	std::vector<std::shared_ptr<Sprite_Divide>>numSprite;
	int maxDigit;

	std::shared_ptr<Sprite_Normal>nextLevelSprite;

	//中央柱に描画する
	static const int LV_MAX_DIGIT = 2;
	std::shared_ptr<Object2D>lvObj2d;
	std::shared_ptr<Object2D>maxObj2d;
	std::shared_ptr<Object2D_Divide>lvNumObj2d[LV_MAX_DIGIT];

	std::shared_ptr<Object2D>upObj2d;
	int upTimer = 0;
	int upDrawTimer = 0;
};