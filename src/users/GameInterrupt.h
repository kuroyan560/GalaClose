#pragma once
#include"../../Singleton.h"
#include<memory>

class Sprite_Normal;

class GameInterrupt : public Singleton<GameInterrupt>
{
	GameInterrupt();
	friend class Singleton<GameInterrupt>;

	std::shared_ptr<Sprite_Normal>blackScreen;
	
	//pauseスプライト
	std::shared_ptr<Sprite_Normal>pauseSprite;

	static const enum ITEM_TYPE{ CONTINUE, RESTART, TITLE, ITEM_NUM };
	std::shared_ptr<Sprite_Normal>itemSprite[ITEM_NUM];

	int select;
	int oldSelect;

	bool on = false;

	int selectSE;
	int enterSE;

	bool restart;

public:
	void Init();
	void Update();
	void Draw();

	void OnOff();
	const bool& GetOn() { return on; }
	const bool& Restart() { return restart; }
};