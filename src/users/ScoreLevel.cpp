#include "ScoreLevel.h"
#include <fstream>
#include <sstream>
#include <string>
#include"PanelMgr.h"
#include"../../SRVDescMgr.h"
#include"../../Sprite.h"
#include"../../Object2D.h"
#include"Constants.h"
#include"../../Easing.h"
#include"../../WinApp.h"

const float SIZE_RATE = 0.3f;
const float LV_NUM_Y = -0.5f;
const float LV_NUM_Z = -6.6f;
const int UP_TOTAL_TIME = 20;
const int UP_DRAW_TOTAL_TIME = 60;

using namespace std;
ScoreLevel::ScoreLevel()
{
	fstream file;

	//ファイルの読み込み
	file.open("resource/scoreLevels.txt");

	//ファイルの読み込み失敗時
	if (file.fail())
	{
		//表示して抜ける
		printf("スコアアタックのレベルが読み込めませんでした\n");
		return;
	}

	//文字列型
	string line;

	int max = 0;

	//一行ずつ文字列を読み取る
	while (getline(file, line))
	{
		vector<int> tmpBorder;
		istringstream lineS(line);
		string borderStr;

		//カンマ区切りで読み取る
		while (getline(lineS, borderStr, ','))
		{
			//読み込んだ値を数値に変換できる形にする
			istringstream str(borderStr);
			int tmp;

			//文字列からぶち込む
			str >> tmp;

			//ベクター配列に突っ込む
			tmpBorder.emplace_back(tmp);

			if (max < tmp)max = tmp;
		}
		//吐き出す
		borders.emplace_back(tmpBorder);
	}

	int handle = SRVDescMgr::Instance()->CreateSRV("resource/img/num.png");
	maxDigit = std::to_string(max).length();

	for (int i = 0; i < maxDigit; ++i)
	{
		numSprite.emplace_back(new Sprite_Divide("ScoreLevelNumSprite", { 10,1 }, { 96,128 }, handle, { 96,128 }, { 0,0 }, false, false, { 0.0f,0.0f }));

		numSprite[i]->GetComponent().transform.pos.y = (128.0f * 0.5f + 10) * WinApp::Instance()->GetWinDifferRate().y;
		numSprite[i]->GetComponent().transform.scale = MyFunc::MulXMFLOAT2({ SIZE_RATE,SIZE_RATE }, WinApp::Instance()->GetWinDifferRate());
	}

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/nextLevel.png");
	nextLevelSprite.reset(new Sprite_Normal("NextLevelSprite", handle, 
		MyDef::float2{ 600.0f * SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x,128.0f * SIZE_RATE * WinApp::Instance()->GetWinDifferRate().y },
		{ 0.0f,(128.0f * 0.5f + 13) * WinApp::Instance()->GetWinDifferRate().y }, false, false, { 0.0f,0.0f }));

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/lv.png");
	lvObj2d.reset(new Object2D("LvObject2D", { 128,128 }, { 0.0f,4.9f,LV_NUM_Z }, handle, { 0.5f,0.5f }));
	lvObj2d->scale.x = 0.06f;
	lvObj2d->scale.y = 0.06f;

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/max.png");
	maxObj2d.reset(new Object2D("MaxObject2D", { 192,128 }, { 0.0f,LV_NUM_Y,LV_NUM_Z }, handle, { 0.5f,0.5f }));
	maxObj2d->scale.x = 0.1f;
	maxObj2d->scale.y = 0.1f;

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/num_white.png");
	for (int i = 0; i < LV_MAX_DIGIT; ++i)
	{
		lvNumObj2d[i].reset(new Object2D_Divide("LvNumObject2D" + to_string(i), { 10,1 }, { 96,128 }, 
			handle,{ 96,128 }, { 0.0f,LV_NUM_Y,LV_NUM_Z }, { 0.5f,0.5f }));
		lvNumObj2d[i]->pos.x = i ? -2.2f : 2.2f;
		lvNumObj2d[i]->scale.x = 0.11f;
		lvNumObj2d[i]->scale.y = 0.11f;
	}

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/up.png");
	upObj2d.reset(new Object2D("UpObject2D", { 160,128 }, { 0.0f,LV_NUM_Y,LV_NUM_Z }, handle, { 0.5f,0.5f }));
	upObj2d->scale.x = 0.05f;
	upObj2d->scale.y = 0.05f;
}

void ScoreLevel::Init(int* ScorePtr)
{
	score = ScorePtr;
	lv = 1;
	lvMax = false;
	upTimer = UP_TOTAL_TIME;
	upObj2d->isAlive = false;
	upDrawTimer = UP_DRAW_TOTAL_TIME;

	upObj2d->GetDirtyFlag().Register();
}

void ScoreLevel::Update()
{
	if (score != nullptr && !lvMax)
	{
		int panelNum = (PanelMgr::Instance()->GetPanelNum() - 4) / 2;
		nowBorder = borders[panelNum][PanelMgr::Instance()->GetMaxLayerNum() - 1];
		//スコアがボーダーより高くなったら
		if (nowBorder <= *score)
		{
			upTimer = 0;
			upObj2d->isAlive = true;
			upDrawTimer = UP_DRAW_TOTAL_TIME;

			if (!PanelMgr::Instance()->AddPanelAndLayerReset())
			{
				lv++;
			}
			else
			{
				//レベルが最大まで来た
				lvMax = true;
			}
		}

		if (upTimer < UP_TOTAL_TIME)
		{
			upObj2d->pos.y = Easing::Ease(Out, Back, upTimer, UP_TOTAL_TIME, float(LV_NUM_Y - 2.5f), float(LV_NUM_Y - 4.5f));

			upTimer++;
			if (upTimer == UP_TOTAL_TIME)
			{
				upDrawTimer = 0;
			}
		}

		if (upDrawTimer < UP_DRAW_TOTAL_TIME)
		{
			upDrawTimer++;
			if (upDrawTimer == UP_DRAW_TOTAL_TIME)
			{
				upObj2d->isAlive = false;
			}
		}
	}
}

void ScoreLevel::DrawObj()
{
	if (score != nullptr)
	{
		lvObj2d->Draw();

		if (lvMax)
		{
			maxObj2d->Draw();
		}
		else
		{
			upObj2d->Draw();

			char lvBuff[25] = { 0 };
			sprintf_s(lvBuff, sizeof(lvBuff), "%d", lv);

			for (int i = 0; i < 2; ++i)
			{
				int idx = i;
				if (10 <= lv)
				{
					idx = 1 - idx;
				}

				if (lvBuff[idx] != '\0')lvBuff[idx] -= 48;

				lvNumObj2d[i]->SelectImgNum(lvBuff[idx], 0);
				lvNumObj2d[i]->Draw();
			}
		}
	}
}

void ScoreLevel::DrawUI()
{
	if (score != nullptr && !lvMax)
	{
		char scoreBuff[25] = { 0 };
		sprintf_s(scoreBuff, sizeof(scoreBuff), "%d", nowBorder);

		int digit = 0;
		int idx = 0;
		while (scoreBuff[idx] != '\0')
		{
			digit++;
			idx++;
		}

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
			scoreBuff[idx] -= 48;

			numSprite[i]->GetComponent().transform.pos.x 
				= 600.0f * SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x + (digit - 1 - i) * 96 * 0.75f * SIZE_RATE * WinApp::Instance()->GetWinDifferRate().x;
			numSprite[i]->SelectImgNum(scoreBuff[idx], 0);
			numSprite[i]->Draw();
		}

		nextLevelSprite->Draw();
	}
}

void ScoreLevel::Finalize()
{
	upObj2d->GetDirtyFlag().Remove();
}
