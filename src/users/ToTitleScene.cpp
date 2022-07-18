#include "ToTitleScene.h"
#include"../../SRVDescMgr.h"
#include"../../Sprite.h"
#include"Constants.h"
#include"../../WinApp.h"
#include"../../Easing.h"

const int APPEAR_TOTAL_TIME = 20;
const int WAIT_TOTAL_TIME = 10;

ToTitleScene::ToTitleScene()
{
	int handle = SRVDescMgr::Instance()->CreateSRV(RED_COLOR.x * 255.0f, RED_COLOR.y * 255.0f, RED_COLOR.z * 255.0f);
	pinkScreen.reset(new Sprite_Normal("PinkScreen", handle, WinApp::Instance()->GetWinSize(), { 0.0f,0.0f }, false, false, { 0.0f,0.0f }));
}

void ToTitleScene::Start()
{
    nowTrans = true;
	pinkScreen->GetComponent().transform.pos.x = -WinApp::Instance()->GetWinSize().x;
	timer = 0;
	flag = APPEAR;
}

bool ToTitleScene::Update()
{
	bool result = false;
	timer++;
	if (flag == APPEAR)
	{
		pinkScreen->GetComponent().transform.pos.x = Easing::Ease(Out, Circ, timer, APPEAR_TOTAL_TIME, -WinApp::Instance()->GetWinSize().x, 0.0f);
		if (timer == APPEAR_TOTAL_TIME)
		{
			flag = WAIT;
			timer = 0;
			result = true;
		}
	}
	else if (flag == WAIT)
	{
		if (timer == WAIT_TOTAL_TIME)
		{
			flag = DISAPPEAR;
			timer = 0;
		}
	}
	else
	{
		pinkScreen->GetComponent().transform.pos.x = Easing::Ease(In, Circ, timer, APPEAR_TOTAL_TIME, 0.0f, WinApp::Instance()->GetWinSize().x);
		if (timer == APPEAR_TOTAL_TIME)
		{
			nowTrans = false;
		}
	}

	return result;
}

void ToTitleScene::Draw()
{
	pinkScreen->Draw();
}
