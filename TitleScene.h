#pragma once
#include"Scene.h"
#include"FixedCamera.h"
#include<memory>
#include"src/users/EnemyGenerateHelper.h"

class TitleEnemy;
class Sprite_Normal;

class TitleScene : public Scene
{
	static const enum { S, T1, A, R, T2, STR_LENGTH };
	EnemyGenerateHelper helper[STR_LENGTH];
	std::shared_ptr<TitleEnemy>titleEnemy[STR_LENGTH];

	int deadNum = 0;
	int nowCam = 1;

	bool changeScene = false;

	FixedCamera cam;

	int bgm;
	int bgmTimer;

	bool shortCut = false;	//startの文字をすぐ消せる位置にスポーン
	int nonOperateTimer = 0;	//無操作時間計測

	//タイトルロゴ
	std::shared_ptr<Sprite_Normal>titleSprite;
	int titleMoveTimer;
	bool titleAppear = true;

	//PressAnyKey
	std::shared_ptr<Sprite_Normal>pressAnyKeySprite;
	float alphaAngle;
	int flashTimer = 0;

	int anyKeySE;

public:
	TitleScene();
	~TitleScene();
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Finalize()override;
};