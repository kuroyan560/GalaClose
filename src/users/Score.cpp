#include "Score.h"
#include"../../MyFunc.h"

#include"../../DebugText.h"
#include <Constants.h>
#include"../../Sprite.h"
#include"../../SRVDescMgr.h"
#include"../../WinApp.h"
#include"../../Easing.h"

const float SIZE_RATE = 0.5f;
const int ADD_SCORE_APPEAR_TOTAL_TIME = 30;
const int ADD_SCORE_WAIT_TOTAL_TIME = 200;
const int ADD_SCORE_MAX_DIGIT = 5;
const float ADD_SCORE_START_X = 360;
const float ADD_SCORE_END_X = 380;
const float ADD_SCORE_SIZE_RATE = 0.3f;

const int SIGN_NUM = 3;
const int RATE_DIGIT = 2;

const float NUM_WIDTH = 96.0f;
const float COMBO_NUM_SIZE = 0.8f;

Score::Score()
{
	int handle = SRVDescMgr::Instance()->CreateSRV("resource/img/num.png");
	for (int i = 0; i < SCORE_MAX_DIGIT; ++i)
	{
		numSprite.emplace_back(new Sprite_Divide("ScoreNumSprite", { 10,1 }, { (int)NUM_WIDTH,128 }, handle, { NUM_WIDTH,128 }, { 0,0 }, false, false, { 0.0f,0.0f }));

		numSprite[i]->GetComponent().transform.pos.x = (SCORE_MAX_DIGIT - 1 - i) * NUM_WIDTH * 0.75f * SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x;
		numSprite[i]->GetComponent().transform.scale.x = SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x;
		numSprite[i]->GetComponent().transform.scale.y = SIZE_RATE * WinApp::Instance()->GetWinDifferRate().y;
	}

	const float OFFSET_X = 25.0f * WinApp::Instance()->GetWinDifferRate().x;

	const float COMBO_STR_X = 60.0f * WinApp::Instance()->GetWinDifferRate().x;
	const float COMBO_STR_X_OFFSET = COMBO_STR_X - 20.0f * WinApp::Instance()->GetWinDifferRate().x;

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/num_sign.png");
	for (int i = 0; i < ADD_SCORE_MAX_DIGIT + SIGN_NUM + RATE_DIGIT; ++i)	//ÇRÅÅãLçÜÇÃêî(+ x .)
	{
		addNumSprite.emplace_back(new Sprite_Divide("AddNumSprite", { 13,1 }, { (int)NUM_WIDTH,128 }, handle, { NUM_WIDTH,128 },
			{ 0.0f, 10.0f }, false, false, { 0.0f,0.0f }));
		addNumSprite[i]->GetComponent().transform.scale = MyFunc::MulXMFLOAT2({ ADD_SCORE_SIZE_RATE,ADD_SCORE_SIZE_RATE }, WinApp::Instance()->GetWinDifferRate());
	/*	comboNumSprite[i]->GetComponent().transform.pos.x = (COMBO_STR_X_OFFSET + (COMBO_MAX_DIGIT - 1 - i)* NUM_WIDTH * 0.75f * COMBO_NUM_SIZE) * WinApp::Instance()->GetWinDifferRate().x + OFFSET_X;
		comboNumSprite[i]->GetComponent().transform.scale = { COMBO_NUM_SIZE * WinApp::Instance()->GetWinDifferRate().x ,COMBO_NUM_SIZE * WinApp::Instance()->GetWinDifferRate().y };*/
	}

	//handle = SRVDescMgr::Instance()->CreateSRV("resource/img/combo.png");
	//comboSprite.reset(new Sprite_Normal("Combo", handle,
	//	MyDef::int2{ 106,54 }, { COMBO_STR_X + OFFSET_X,COMBO_STR_Y + OFFSET_Y }, false, false, { 0.0f,0.0f }));
}

void Score::Init()
{
	score = 0;
	ko = 0;
	drawScore = 0;
	//addScore = 0;

	addScoreTimer = ADD_SCORE_APPEAR_TOTAL_TIME;
	waitTimer = ADD_SCORE_WAIT_TOTAL_TIME;
}

void Score::Update()
{
	if (score != drawScore)
	{
		float buff = drawScore;
		buff = MyFunc::Lerp(drawScore, score, 0.1f);
		if (abs(buff - score) < 0.1f)
		{
			buff = score;
		}
		drawScore = (int)buff;
	}
	if (addScoreTimer < ADD_SCORE_APPEAR_TOTAL_TIME)
	{
		addScoreTimer++;
		addScoreX =
			Easing::Ease(Out, Back, addScoreTimer, ADD_SCORE_APPEAR_TOTAL_TIME,
			ADD_SCORE_START_X * WinApp::Instance()->GetWinDifferRate().x,
			ADD_SCORE_END_X * WinApp::Instance()->GetWinDifferRate().x);
	}
	else
	{
		if (waitTimer < ADD_SCORE_WAIT_TOTAL_TIME)
		{
			waitTimer++;
			//if (waitTimer == ADD_SCORE_WAIT_TOTAL_TIME)
			//{
			//	addScoreTimer = 0;
			//}
		}
	}
}

void Score::Draw()
{
	char scoreBuff[25] = { 0 };
	int digit = to_string(drawScore).length();
	sprintf_s(scoreBuff, sizeof(scoreBuff), "%d", drawScore);

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
		numSprite[i]->SelectImgNum(scoreBuff[idx], 0);
		numSprite[i]->Draw();
	}

	if (addScoreTimer != ADD_SCORE_APPEAR_TOTAL_TIME
		|| waitTimer != ADD_SCORE_WAIT_TOTAL_TIME)
	{
		float x = addScoreX;
		int nowDigit = 0;
		const float OFFSET_X = NUM_WIDTH * ADD_SCORE_SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x * 0.8f;
		//Å{
		addNumSprite[0]->SelectImgNum(10, 0);
		addNumSprite[0]->GetComponent().transform.pos.x = x;
		x += OFFSET_X;
		nowDigit++;

		//í«â¡ÉXÉRÉA
		char addScoreBuff[25] = { 0 };
		int addScoredigit = to_string(100).length();
		sprintf_s(addScoreBuff, sizeof(addScoreBuff), "%d", 100);
		for (int i = 1; i < addScoredigit + 1; ++i)
		{
			int idx = i;
			//if (i < addScoredigit + 1)
			//{
			//	idx = addScoredigit - i;
			//}
			//else
			//{
			//	idx = i;
			//}
			if (addScoreBuff[idx - 1] != '\0')addScoreBuff[idx - 1] -= 48;
			addNumSprite[i]->GetComponent().transform.pos.x = x;
			addNumSprite[i]->SelectImgNum(addScoreBuff[idx - 1], 0);
			x += OFFSET_X;
			nowDigit++;
		}

		if (1 < ko)
		{
			//Å~
			addNumSprite[1 + addScoredigit]->SelectImgNum(11, 0);
			addNumSprite[1 + addScoredigit]->GetComponent().transform.pos.x = x;
			x += OFFSET_X;
			nowDigit++;

			//î{ó¶êÆêî
			char koBuff[25] = { 0 };
			digit = to_string(ko - 1).length();
			sprintf_s(koBuff, sizeof(koBuff), "%d", ko - 1);

			for (int i = 0; i < digit; ++i)
			{
				if (koBuff[i] != '\0')koBuff[i] -= 48;
			}

			addNumSprite[2 + addScoredigit]->SelectImgNum(ko - 1 < 10 ? 1 : ((ko - 1) / 10 + 1), 0);
			addNumSprite[2 + addScoredigit]->GetComponent().transform.pos.x = x;
			x += OFFSET_X;
			nowDigit++;

			//.
			addNumSprite[3 + addScoredigit]->SelectImgNum(12, 0);
			addNumSprite[3 + addScoredigit]->GetComponent().transform.pos.x = x;
			x += OFFSET_X;
			nowDigit++;

			//î{ó¶è≠êî
			addNumSprite[4 + addScoredigit]->SelectImgNum(ko - 1 < 10 ? koBuff[0] : koBuff[1], 0);
			addNumSprite[4 + addScoredigit]->GetComponent().transform.pos.x = x;
			x += OFFSET_X;
			nowDigit++;
		}

		for (int i = 0; i < nowDigit; ++i)
		{
			addNumSprite[i]->Draw();
		}
	}

	//DebugText::Instance()->FormatPrint(0, 0, 1.2f, "score : %d", score);
	//DebugText::Instance()->FormatPrint(0, 1.2f * 16.0f, 1.2f, "combo : %d", combo);
}

void Score::Add()
{
	//addScore = KILL_GET_SCORE * (1.0f + COMBO_ADD_RATE * ko);
	score += KILL_GET_SCORE * (1.0f + COMBO_ADD_RATE * ko);
	if (SCORE_MAX_DIGIT < std::to_string(score).length())
	{
		score = 9999999999;
	}
	ko++;
	addScoreTimer = 0;
	waitTimer = 0;
}
