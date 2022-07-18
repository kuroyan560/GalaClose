#include "GameInterrupt.h"
#include"../../SRVDescMgr.h"
#include"../../Sprite.h"
#include"../../WinApp.h"
#include"../../UsersInput.h"
#include"../../SceneMgr.h"
#include"../../AudioMgr.h"

GameInterrupt::GameInterrupt()
{
	int handle = SRVDescMgr::Instance()->CreateSRV(0, 0, 0);
	blackScreen.reset(new Sprite_Normal("BlackScreen", handle, WinApp::Instance()->GetWinSize(), { 0.0f,0.0f }, false, false, { 0.0f,0.0f }));
	blackScreen->GetAlpha() = 0.5f;

	const int PAUSE_POS_Y = 130;
	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/pause.png");
	pauseSprite.reset(new Sprite_Normal("Pause", handle, MyDef::int2{ 445,96 },
		{ WinApp::Instance()->GetWinCenter().x,PAUSE_POS_Y * WinApp::Instance()->GetWinDifferRate().y }, false, false, { 0.5f,0.5f }));

	const int START_OFFSET = 180;
	const int ITEM_OFFSET = 140;

	std::string spriteName[ITEM_NUM] =
	{
		"Pause_Continue","Pause_Restart","Pause_Title"
	};
	std::string file[ITEM_NUM] =
	{
		"resource/img/pause_continue.png",
		"resource/img/pause_restart.png",
		"resource/img/pause_title.png"
	};
	for (int i = 0; i < ITEM_NUM; ++i)
	{
		handle = SRVDescMgr::Instance()->CreateSRV(file[i]);
		itemSprite[i].reset(new Sprite_Normal(spriteName[i], handle,
			MyDef::int2{ 344,105 }, { WinApp::Instance()->GetWinCenter().x,0.0f }, false, false, { 0.5f,0.5f }));

		itemSprite[i]->GetComponent().transform.pos.y = (PAUSE_POS_Y + START_OFFSET + ITEM_OFFSET * i) * WinApp::Instance()->GetWinDifferRate().y;
	}

	selectSE = AudioMgr::Instance()->LoadAudio("resource/sound/selectSE.wav");
	enterSE = AudioMgr::Instance()->LoadAudio("resource/sound/pushResetSE.wav");
}

void GameInterrupt::Init()
{
	on = false;
	select = ITEM_NUM;
	oldSelect = ITEM_NUM;

	for (int i = 0; i < ITEM_NUM; ++i)
	{
		itemSprite[i]->GetComponent().transform.scale = { 0.7f,0.7f };
		itemSprite[i]->GetAlpha() = 0.3f;
		itemSprite[i]->GetComponent().ColorMapping();
		itemSprite[i]->GetComponent().MatMapping();

		itemSprite[i]->GetComponent().DirtyRegister();
	}

	restart = false;
}

void GameInterrupt::Update()
{
	if (on && !restart)
	{
		if (0 < select && UsersInput::Instance()->OnTrigger(DIK_UP))
		{
			select--;
		}
		else if (select < ITEM_NUM - 1 && UsersInput::Instance()->OnTrigger(DIK_DOWN))
		{
			select++;
		}

		if (UsersInput::Instance()->OnTrigger(DIK_SPACE))
		{
			AudioMgr::Instance()->PlayWave(enterSE);

			if (select == CONTINUE)
			{
				this->OnOff();
			}
			else if (select == RESTART)
			{
				restart = true;
				//GameCamera::Instance()->ZoomToCenter();
				SceneMgr::Instance()->ChangeGameScene();
			}
			else
			{
				SceneMgr::Instance()->ChangeTitleScene();
			}
		}

		if (oldSelect != select)
		{
			AudioMgr::Instance()->PlayWave(selectSE);

			if (oldSelect != ITEM_NUM)
			{
				itemSprite[oldSelect]->GetComponent().transform.scale = { 0.7f,0.7f };
				itemSprite[oldSelect]->GetAlpha() = 0.3f;
			}

			itemSprite[select]->GetComponent().transform.scale = { 1.0f,1.0f };
			itemSprite[select]->GetAlpha() = 1.0f;
		}
		oldSelect = select;
	}

	//if (restart)
	//{
	//	if (!GameCamera::Instance()->NowMotion())
	//	{
	//		SceneMgr::Instance()->ChangeScene(1);
	//	}
	//}
}

void GameInterrupt::Draw()
{
	if (on && !restart)
	{
		blackScreen->Draw();

		pauseSprite->Draw();
		for (int i = 0; i < ITEM_NUM; ++i)
		{
			itemSprite[i]->Draw();
		}
	}
}

void GameInterrupt::OnOff()
{
	on = !on;

	if (on)
	{
		select = CONTINUE;
	}
}
