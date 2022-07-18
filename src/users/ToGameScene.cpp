#include "ToGameScene.h"
#include"../../GameCamera.h"
#include"../../Sprite.h"
#include"../../SRVDescMgr.h"
#include"../../WinApp.h"

const int READY_TOTAL_TIME = 50;

ToGameScene::ToGameScene()
{
	int handle = SRVDescMgr::Instance()->CreateSRV("resource/img/ready.png");
	ready.reset(new Sprite_Normal("Ready", handle, MyDef::int2{ 391,117 }, WinApp::Instance()->GetWinCenter(), false, false, { 0.5f,0.5f }));
}

void ToGameScene::Start()
{
	nowTrans = true;
	GameCamera::Instance()->ZoomToCenter();
	timer = 0;

	ready->GetComponent().transform.scale = { 0.0f,0.0f };
	ready->GetComponent().transform.rotate.x = 360.0f * 2;
}

bool ToGameScene::Update()
{
	if (timer < READY_TOTAL_TIME)
	{
		timer++;
		ready->GetComponent().transform.scale = Easing::Ease(Out, Elastic, timer, READY_TOTAL_TIME, { 0.0f,0.0f }, { 1.5f,1.5f });
		ready->GetComponent().transform.rotate.x = Easing::Ease(Out, Quint, timer, READY_TOTAL_TIME, 360.0f * 2, 0.0f);
	}

	if (!GameCamera::Instance()->NowMotion())
	{
		nowTrans = false;
		return true;
	}
	return false;
}

void ToGameScene::Draw()
{
	ready->Draw();
}