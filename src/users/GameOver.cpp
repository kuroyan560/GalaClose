#include "GameOver.h"
#include"EnemyMgr.h"
#include"QueenMgr.h"
#include"PanelMgr.h"
#include"../../Easing.h"
#include"../../SRVDescMgr.h"
#include"../../Sprite.h"
#include"Score.h"
#include"EnemyKillEvent.h"
#include"../../AudioMgr.h"
#include"../../UsersInput.h"
#include"../../SceneMgr.h"
#include"../../GameCamera.h"

const int PANEL_EXPLOSION_TOTAL_TIME = 110;
const float EXPLOSION_MIN = 1.0f;
const float EXPLOSION_MAX = 3.0f;
const int APPEAR_TOTAL_TIME = 60;
const int INFO_DRAW_SPAN = 30;

const float SIZE_RATE = 0.5f;

const int EXPLOSION_WAIT_TOTAL_TIME = 125;

GameOver::GameOver()
{
	int handle = SRVDescMgr::Instance()->CreateSRV(0, 0, 0);
	blackBack.reset(new Sprite_Normal("BlackBack", handle, WinApp::Instance()->GetWinSize(), { 0.0f,0.0f }, false, false, { 0.0f,0.0f }));

	const float GAME_OVER_Y = 100 * WinApp::Instance()->GetWinDifferRate().y;
	const float STR_X = 300 * WinApp::Instance()->GetWinDifferRate().x;
	const float TOTAL_SCORE_Y = 240 * WinApp::Instance()->GetWinDifferRate().y;
	const float ENEMY_KILL_Y = 340 * WinApp::Instance()->GetWinDifferRate().y;
	const float ITEM_X_OFFSET = 220.0f * WinApp::Instance()->GetWinDifferRate().x;
	const float ITEM_Y = 550.0f * WinApp::Instance()->GetWinDifferRate().y;

	const float NUM_POS_X = 300.0f * WinApp::Instance()->GetWinDifferRate().x + STR_X;

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/gameover.png");
	gameOverSprite.reset(new Sprite_Normal("GameOver", handle, MyDef::int2{ 532,85 },
		{ WinApp::Instance()->GetWinCenter().x,GAME_OVER_Y }, false, false, { 0.5f,0.0f }));
	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/totalScore.png");
	totalScoreSprite.reset(new Sprite_Normal("TotalScore", handle, MyDef::int2{ 258,39 },
		{ STR_X,TOTAL_SCORE_Y }, false, false, { 0.0f,0.0f }));
	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/enemyKill.png");
	enemyKillSprite.reset(new Sprite_Normal("EnemyKill", handle, MyDef::int2{ 228,39 },
		{ STR_X,ENEMY_KILL_Y }, false, false, { 0.0f,0.0f }));
	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/pause_restart.png");
	restartSprite.reset(new Sprite_Normal("GameOverRestart", handle, MyDef::int2{ 344,105 },
		{ WinApp::GetWinCenter().x - ITEM_X_OFFSET,ITEM_Y }, false, false, { 0.5f,0.0f }));
	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/pause_title.png");
	titleSprite.reset(new Sprite_Normal("GameOverTitle", handle, MyDef::int2{ 344,105 },
		{ WinApp::GetWinCenter().x + ITEM_X_OFFSET,ITEM_Y }, false, false, { 0.5f,0.0f }));

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/num.png");
	const float NUM_OFFSET_Y = -10.0f;
	for (int i = 0; i < SCORE_MAX_DIGIT; ++i)
	{
		numTotalScoreSprite.emplace_back(new Sprite_Divide("ScoreNumSprite", { 10,1 }, { 96,128 }, handle, { 96,128 }, { 0,0 }, false, false, { 0.0f,0.0f }));
		numTotalScoreSprite[i]->GetComponent().transform.pos.x = (SCORE_MAX_DIGIT - 1 - i) * 96 * 0.75f * SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x + NUM_POS_X;
		numTotalScoreSprite[i]->GetComponent().transform.pos.y = TOTAL_SCORE_Y + NUM_OFFSET_Y;
		numTotalScoreSprite[i]->GetComponent().transform.scale = MyFunc::MulXMFLOAT2(WinApp::Instance()->GetWinDifferRate(), { SIZE_RATE,SIZE_RATE });

		numEnemyKillSprite.emplace_back(new Sprite_Divide("KillEnemyNumSprite", { 10,1 }, { 96,128 }, handle, { 96,128 }, { 0,0 }, false, false, { 0.0f,0.0f }));
		numEnemyKillSprite[i]->GetComponent().transform.pos.x = (SCORE_MAX_DIGIT - 1 - i) * 96 * 0.75f * SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x + NUM_POS_X;
		numEnemyKillSprite[i]->GetComponent().transform.pos.y = ENEMY_KILL_Y + NUM_OFFSET_Y;
		numEnemyKillSprite[i]->GetComponent().transform.scale = MyFunc::MulXMFLOAT2(WinApp::Instance()->GetWinDifferRate(), { SIZE_RATE,SIZE_RATE });
	}

	drawSE = AudioMgr::Instance()->LoadAudio("resource/sound/pushResetSE.wav");
	selectSE = AudioMgr::Instance()->LoadAudio("resource/sound/selectSE.wav");
	explosionSE = AudioMgr::Instance()->LoadAudio("resource/sound/explosion.wav");
	readyExplosionSE = AudioMgr::Instance()->LoadAudio("resource/sound/readyExplosion.wav");
}

void GameOver::Explosion()
{
	EnemyMgr::Instance()->AllDead();
	QueenMgr::Instance()->AllDead();
	panelExplosionTimer = 0;

	for (int i = 0; i < PanelMgr::Instance()->GetPanelNum(); ++i)
	{
		auto& panel = PanelMgr::Instance()->GetPanelByIndex(i);
		panel->RegisterLayerBeforeExplosion();
		explosionLayerRate.emplace_back(MyFunc::GetRand(EXPLOSION_MIN, EXPLOSION_MAX));
	}

	slowTimer = 0;
	slowSpan = 1.0f;

	selectInfoFlag = -1;
	infoDrawTimer = INFO_DRAW_SPAN;

	selectItem = 0;

	titleSprite->GetAlpha() = 0.3f;
	titleSprite->GetComponent().transform.scale = { 0.7f,0.7f };
	titleSprite->GetComponent().ColorMapping();
	titleSprite->GetComponent().MatMapping();

	restartSprite->GetAlpha() = 1.0f;
	restartSprite->GetComponent().transform.scale = { 1.0f,1.0f };
	restartSprite->GetComponent().ColorMapping();
	restartSprite->GetComponent().MatMapping();

	AudioMgr::Instance()->PlayWave(explosionSE);
}

void GameOver::Init()
{
	flag = false;
	panelExplosionTimer = PANEL_EXPLOSION_TOTAL_TIME;
	explosionLayerRate.clear();
	blackBack->GetAlpha() = 0.0f;

	appearTimer = APPEAR_TOTAL_TIME;

	titleSprite->GetComponent().DirtyRegister();
	restartSprite->GetComponent().DirtyRegister();

	restart = false;

	explosionWaitTimer = EXPLOSION_WAIT_TOTAL_TIME;
}

void GameOver::Update()
{
	if (/*!GameCamera::Instance()->NowMotion() && */explosionWaitTimer < EXPLOSION_WAIT_TOTAL_TIME)
	{
		explosionWaitTimer++;
		if (explosionWaitTimer == EXPLOSION_WAIT_TOTAL_TIME)
		{
			Explosion();
			GameCamera::Instance()->ExplosionCam();
		}
	}

	if (flag && explosionWaitTimer == EXPLOSION_WAIT_TOTAL_TIME)
	{
		if (infoDrawTimer < INFO_DRAW_SPAN)
		{
			infoDrawTimer++;
		}

		if (selectInfoFlag == -1)
		{
			//パネル爆発とスロー
			slowTimer++;
			if (panelExplosionTimer < PANEL_EXPLOSION_TOTAL_TIME)
			{
				panelExplosionTimer++;

				slowSpan = Easing::Ease(Out, Circ,
					panelExplosionTimer, PANEL_EXPLOSION_TOTAL_TIME, 1.0f, 7.0f);

				if (panelExplosionTimer == PANEL_EXPLOSION_TOTAL_TIME)
				{
					appearTimer = 0;
				}
			}
			else
			{
				slowSpan = MyFunc::Lerp(slowSpan, 10.0f, 0.005f);
			}

			//選択画面
			if (appearTimer < APPEAR_TOTAL_TIME)
			{
				appearTimer++;
				blackBack->GetAlpha() = Easing::Ease(InOut, Cubic, appearTimer, APPEAR_TOTAL_TIME, 0.0f, 0.7f);
				if (appearTimer == APPEAR_TOTAL_TIME)
				{
					selectInfoFlag = 0;
					infoDrawTimer = 0;
				}
			}
		}
		else if (selectInfoFlag == 0)
		{
			//トータルスコア表示までの時間計測
			if (infoDrawTimer == INFO_DRAW_SPAN)
			{
				selectInfoFlag++;
				infoDrawTimer = 0;
				AudioMgr::Instance()->PlayWave(drawSE);
			}
		}
		else if (selectInfoFlag == 1)
		{
			//倒した敵の数表示までの時間計測
			if (infoDrawTimer == INFO_DRAW_SPAN)
			{
				selectInfoFlag++;
				infoDrawTimer = 0;
				AudioMgr::Instance()->PlayWave(drawSE);
			}
		}
		else if (selectInfoFlag == 2)
		{
			//選択肢の表示までの時間計測
			if (infoDrawTimer == INFO_DRAW_SPAN)
			{
				selectInfoFlag++;
				AudioMgr::Instance()->PlayWave(drawSE);
			}
		}
		else if (selectInfoFlag == 3 && !restart)
		{
			//選択許容
			//タイトル選択
			if (selectItem == 0 && UsersInput::Instance()->OnTrigger(DIK_RIGHT))
			{
				selectItem = 1;
				titleSprite->GetAlpha() = 1.0f;
				titleSprite->GetComponent().transform.scale = { 1.0f,1.0f };
				restartSprite->GetAlpha() = 0.3f;
				restartSprite->GetComponent().transform.scale = { 0.7f,0.7f };
				AudioMgr::Instance()->PlayWave(selectSE);
			}
			//リスタート選択
			else if (selectItem == 1 && UsersInput::Instance()->OnTrigger(DIK_LEFT))
			{
				selectItem = 0;
				titleSprite->GetAlpha() = 0.3f;
				titleSprite->GetComponent().transform.scale = { 0.7f,0.7f };
				restartSprite->GetAlpha() = 1.0f;
				restartSprite->GetComponent().transform.scale = { 1.0f,1.0f };
				AudioMgr::Instance()->PlayWave(selectSE);
			}

			if (UsersInput::Instance()->OnTrigger(DIK_SPACE))
			{
				AudioMgr::Instance()->PlayWave(drawSE);
				if (selectItem == 0) //リスタート
				{
					SceneMgr::Instance()->ChangeGameScene();
					restart = true;
				}
				else //タイトル
				{
					SceneMgr::Instance()->ChangeTitleScene();
				}
			}
		}
		else
		{
		}
	}
}

void GameOver::Draw()
{
	if (flag && !restart && explosionWaitTimer == EXPLOSION_WAIT_TOTAL_TIME)
	{
		blackBack->Draw();

		//選択画面
		if (0 <= selectInfoFlag)
		{
			gameOverSprite->Draw();
			totalScoreSprite->Draw();
			enemyKillSprite->Draw();
		}

		//スコア
		if (1 <= selectInfoFlag)
		{
			int score = *Score::Instance()->GetScorePtr();
			char scoreBuff[25] = { 0 };
			int digit = to_string(score).length();
			sprintf_s(scoreBuff, sizeof(scoreBuff), "%d", score);

			for (int i = 0; i < SCORE_MAX_DIGIT; ++i)
			{
				int idx;
				if (i < digit)
				{
					idx = digit - 1 - i;
				}
				else
				{
					idx = i;
				}
				if (scoreBuff[idx] != '\0')scoreBuff[idx] -= 48;
				numTotalScoreSprite[i]->SelectImgNum(scoreBuff[idx], 0);
				numTotalScoreSprite[i]->Draw();
			}
		}

		//キルエネミー
		if (2 <= selectInfoFlag)
		{
			int killEnemyNum = EnemyKillEvent::Instance()->GetKillEnemyNum();

			char killEnemyBuff[25] = { 0 };
			int digit = to_string(killEnemyNum).length();
			sprintf_s(killEnemyBuff, sizeof(killEnemyBuff), "%d", killEnemyNum);

			for (int i = 0; i < digit; ++i)
			{
				int idx;
				if (i < digit)
				{
					idx = digit - 1 - i;
				}
				else
				{
					idx = i;
				}
				if (killEnemyBuff[idx] != '\0')killEnemyBuff[idx] -= 48;
				numEnemyKillSprite[i]->SelectImgNum(killEnemyBuff[idx], 0);
				numEnemyKillSprite[i]->Draw();
			}
		}

		//選択肢
		if (3 <= selectInfoFlag)
		{
			restartSprite->Draw();
			titleSprite->Draw();
		}
	}
}

void GameOver::AffectPushedRate(const int& OldPushedLayer, const int& PanelIndex, float* PushedRate)
{
	if (flag && explosionWaitTimer == EXPLOSION_WAIT_TOTAL_TIME)
	{
		*PushedRate = Easing::Ease(Out, Exp,
			panelExplosionTimer, PANEL_EXPLOSION_TOTAL_TIME, (float)OldPushedLayer, -explosionLayerRate[PanelIndex]);
	}
}

void GameOver::AffectPushedRate(const float From, const float To, float* Target)
{
	if (flag && explosionWaitTimer == EXPLOSION_WAIT_TOTAL_TIME)
	{
		*Target = Easing::Ease(Out, Exp,
			panelExplosionTimer, PANEL_EXPLOSION_TOTAL_TIME, From, To);
	}
}

void GameOver::Launch(const DirectX::XMFLOAT3& DeadEnemyPos)
{
	if (!flag)
	{
		GameCamera::Instance()->ExplosionCam(DeadEnemyPos);
		AudioMgr::Instance()->PlayWave(readyExplosionSE);

		flag = true;
		explosionWaitTimer = 0;
	}
}

bool GameOver::Slow()
{
	if (!flag || explosionWaitTimer < EXPLOSION_WAIT_TOTAL_TIME)return false;

	return slowTimer % (int)slowSpan != 0;
}