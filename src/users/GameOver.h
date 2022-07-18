#pragma once
#include"../../Singleton.h"
#include<vector>
#include<memory>
#include<DirectXMath.h>

class Sprite_Normal;
class Sprite_Divide;

class GameOver : public Singleton<GameOver>
{
	friend class Singleton<GameOver>;
	GameOver();
	bool flag;

	int panelExplosionTimer;	//パネルが破壊される時間
	std::vector<float>explosionLayerRate;

	int slowTimer;
	float slowSpan;

	//演出後
	std::shared_ptr<Sprite_Normal>blackBack;
	int appearTimer = 0;

	//選択画面
	int selectInfoFlag;
	int infoDrawTimer;
	std::shared_ptr<Sprite_Normal>gameOverSprite;
	std::shared_ptr<Sprite_Normal>totalScoreSprite;
	std::shared_ptr<Sprite_Normal>enemyKillSprite;
	std::shared_ptr<Sprite_Normal>restartSprite;
	std::shared_ptr<Sprite_Normal>titleSprite;
	std::vector<std::shared_ptr<Sprite_Divide>>numTotalScoreSprite;
	std::vector<std::shared_ptr<Sprite_Divide>>numEnemyKillSprite;

	int drawSE;
	int selectSE;
	int selectItem = 0;
	int explosionSE;
	int readyExplosionSE;

	bool restart;

	void Explosion();
	int explosionWaitTimer = 0;

public:
	void Init();
	void Update();
	void Draw();
	void AffectPushedRate(const int& OldPushedLayer, const int& PanelIndex, float* PushedRate);
	void AffectPushedRate(const float From, const float To, float* Target);
	void Finalize() { flag = false; }

	void Launch(const DirectX::XMFLOAT3& DeadEnemyPos);
	bool Flag() { return flag; }
	bool Slow();
};