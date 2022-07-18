#include "Tutorial.h"
#include"../../SRVDescMgr.h"
#include"../../Sprite.h"
#include"Constants.h"
#include"../../Easing.h"
#include"../../WinApp.h"
#include"Player.h"
const int MOVE_TOTAL_TIME = 30;
const int GRAPH_WIDTH = 454;
const float BIG_SCALE = 0.8f;
const float SMALE_SCALE = 0.5f;

Tutorial::Tutorial()
{
	int handle = SRVDescMgr::Instance()->CreateSRV("resource/img/tutorial.png");
	tutorialSprite.reset(
		new Sprite_Normal("Tutorial", handle, MyDef::int2{ 454,144 }, { (float)WinApp::GetWinSize().x,(float)WinApp::GetWinSize().y }, false, false, { 1.0f,1.0f }));
	tutorialSprite->GetComponent().transform.scale = { 0.8f,0.8f };
	offsetX = WinApp::Instance()->GetWinDifferRate().x * GRAPH_WIDTH;

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/pushPower.png");
	pushPowerSprite.reset(
		new Sprite_Normal("PushPower", handle, MyDef::int2{ 275,37 }, { (float)WinApp::GetWinSize().x-90 * WinApp::Instance()->GetWinDifferRate().x,
			5.0f * WinApp::Instance()->GetWinDifferRate().y }, false, false, { 1.0f,0.0f }));

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/power.png");
	for (int i = 0; i < LAYER_NUM_MAX_LIMIT; ++i)
	{
		powerSprite.emplace_back(new Sprite_Normal("Power", handle, MyDef::int2{ 62,62 }, 
			{ (float)WinApp::GetWinSize().x + (-7.0f - 31.0f) * WinApp::Instance()->GetWinDifferRate().x,
			((float)i * 62 + 31 + 5.0f) * WinApp::Instance()->GetWinDifferRate().y }, false, false, { 0.5f,0.5f }));
	}

	const int GLAY_IDX = 0;
	handle = SRVDescMgr::Instance()->CreateSRV(
		GLAY_COLORS[GLAY_IDX].x * 255.0f, GLAY_COLORS[GLAY_IDX].y * 255.0f, GLAY_COLORS[GLAY_IDX].z * 255.0f);
	powerBackSprite.reset(
		new Sprite_Normal("PowerBack", handle, MyDef::int2{ 80,64 * LAYER_NUM_MAX_LIMIT }, { (float)WinApp::Instance()->GetWinSize().x,0.0f }, false, false, { 1.0f,0.0f }));
}

void Tutorial::Init(const bool& Hide)
{
	hide = Hide;

	if (hide)
	{
		tutorialSprite->GetComponent().transform.pos.x = WinApp::GetWinSize().x + offsetX;
	}
	else
	{
		tutorialSprite->GetComponent().transform.pos.x = WinApp::GetWinSize().x;
	}
	
	timer = MOVE_TOTAL_TIME;

	for (auto& power : powerSprite)
	{
		power->GetComponent().DirtyRegister();
		power->GetComponent().transform.scale = { SMALE_SCALE,SMALE_SCALE };
		power->GetComponent().color.color.x = 0.8f;
		power->GetComponent().color.color.y = 0.8f;
		power->GetComponent().color.color.z = 0.8f;
		power->GetComponent().ColorMapping();
		power->GetComponent().MatMapping();
	}

	oldPower = -1;
}

void Tutorial::Update()
{
	int nowPower = Player::Instance()->GetPushPower();
	if (oldPower != nowPower)
	{
		for (int i = 0; i < LAYER_NUM_MAX_LIMIT; ++i)
		{
			if (i < nowPower)
			{
				powerSprite[i]->GetComponent().transform.scale = { BIG_SCALE,BIG_SCALE };
				powerSprite[i]->GetComponent().color.color.x = 1.0f;
				powerSprite[i]->GetComponent().color.color.y = 1.0f;
				powerSprite[i]->GetComponent().color.color.z = 1.0f;
			}
			else
			{
				powerSprite[i]->GetComponent().transform.scale = { SMALE_SCALE,SMALE_SCALE };
				powerSprite[i]->GetComponent().color.color.x = 0.8f;
				powerSprite[i]->GetComponent().color.color.y = 0.8f;
				powerSprite[i]->GetComponent().color.color.z = 0.8f;
			}
		}
	}

	if (timer < MOVE_TOTAL_TIME)
	{
		timer++;

		if (hide)
		{
			tutorialSprite->GetComponent().transform.pos.x
				= Easing::Ease(In, Back, timer, MOVE_TOTAL_TIME, WinApp::GetWinSize().x, WinApp::GetWinSize().x + offsetX);
		}
		else
		{
			tutorialSprite->GetComponent().transform.pos.x
				= Easing::Ease(Out, Back, timer, MOVE_TOTAL_TIME, WinApp::GetWinSize().x + offsetX, WinApp::GetWinSize().x);
		}
	}

	oldPower = nowPower;
}

void Tutorial::Draw()
{
	if (!hide)
	{
		tutorialSprite->Draw();

		powerBackSprite->Draw();
		for (auto& power : powerSprite)
		{
			power->Draw();
		}
		pushPowerSprite->Draw();
	}
}

void Tutorial::Move()
{
	hide = !hide;
	timer = 0;
}
