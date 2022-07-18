#include "TitleScene.h"
#include"CameraMgr.h"
#include"Sprite.h"
#include"SRVDescMgr.h"
#include"MyFunc.h"
#include"SceneMgr.h"
#include"UsersInput.h"
#include"Sprite.h"
#include"WinApp.h"
#include"AudioMgr.h"
#include"SceneMgr.h"
#include"Object3D.h"
#include"EffectMgr.h"
#include"GameCamera.h"
#include"src/users/PanelMgr.h"
#include"src/users/BackGround.h"
#include"DebugText.h"
#include"RenderTargetMgr.h"
#include "Action.h"
#include "HowToOperate.h"
#include"src/users/TitleEnemy.h"
#include"src/users/Tutorial.h"

const int NON_OPERATE_TOTAL_TIME = 300;	//無操作の状態がこの時間だけ続いたらタイトル画面Init（タイトル表示）
const int TITLE_MOVE_TOTAL_TIME = 60;
const int FLASH_SPAN = 60;
const int TITLE_LOGO_OFFSET_Y = 30;

TitleScene::TitleScene()
	:Scene("TitleScene")
{
	std::string dir = "resource/model/start/";
	titleEnemy[S].reset(new TitleEnemy(dir, "s.fbx"));
	titleEnemy[T1].reset(new TitleEnemy(dir, "t.fbx"));
	titleEnemy[A].reset(new TitleEnemy(dir, "a.fbx"));
	titleEnemy[R].reset(new TitleEnemy(dir, "r.fbx"));
	titleEnemy[T2].reset(new TitleEnemy(dir, "t.fbx"));

	for (int i = 0; i < STR_LENGTH; ++i)
	{
		helper[i].layerNum = i;
	}

	bgm = AudioMgr::Instance()->LoadAudio("resource/sound/bgm2.wav");

	int handle = SRVDescMgr::Instance()->CreateSRV("resource/img/logo.png");
	titleSprite.reset(new Sprite_Normal("TitleLogo", handle, int2{ 800,660 }, WinApp::Instance()->GetWinCenter(), false, false, { 0.5f,0.5f }));

	handle = SRVDescMgr::Instance()->CreateSRV("resource/img/pressAnyKey.png");
	pressAnyKeySprite.reset(
		new Sprite_Normal("PressAnyKey", handle, int2{ 590,66 }, 
			{ WinApp::Instance()->GetWinCenter().x, 660 * WinApp::Instance()->GetWinDifferRate().y }, false, false, { 0.5f,0.5f }));
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	Object3D::InitAll();
	ActivityEditor_3D::ActivesInit();
	ActivityEditor_2D::ActivesInit();
	ActivityEditor_1D::ActivesInit();
	//CameraMgr::Instance()->RegisterCam(&cam);

	EffectMgr::Instance()->Init();

	CameraMgr::Instance()->RegisterCam(GameCamera::Instance());
	CameraMgr::Instance()->SetCam(1);
	CameraMgr::Instance()->Init();

	PanelMgr::Instance()->Init(6, 5, true);
	for (auto ptr : GameObjectInterFace::objs)
	{
		ptr->Init();
	}


	for (int i = 0; i < STR_LENGTH; ++i)
	{
		if (shortCut)
		{
			helper[i].posAngle = PI * 2 / 6 * 0.5f;
		}
		else
		{
			helper[i].posAngle = PI * 2 / 6 * ((float)i + 0.5f);
		}
		titleEnemy[i]->Spawn(helper[i]);
	}

	BackGround::Instance()->Init();

	deadNum = 0;
	changeScene = false;

	AudioMgr::Instance()->PlayWave(bgm, true);
	AudioMgr::Instance()->ChangeVolume(bgm, 1.0f);
	bgmTimer = ZOOM_CAM_MOTION_TOTAL_TIME;

	nonOperateTimer = NON_OPERATE_TOTAL_TIME;

	titleAppear = true;
	titleMoveTimer = TITLE_MOVE_TOTAL_TIME;
	titleSprite->GetComponent().transform.pos.y = WinApp::Instance()->GetWinCenter().y - TITLE_LOGO_OFFSET_Y;

	alphaAngle = PI;
	flashTimer = 0;

	Tutorial::Instance()->Init(true);

	if (!shortCut)
	{
		shortCut = true;
	}
}

void TitleScene::Update()
{
	HowToOperate::Update(true);

	Action::AllActionUpdate();

	ActivityEditor_3D::ActivesUpdate();
	ActivityEditor_2D::ActivesUpdate();
	ActivityEditor_1D::ActivesUpdate();

	for (auto ptr : GameObjectInterFace::objs)
	{
		ptr->Update();
	}
	BackGround::Instance()->Update();
	PanelMgr::Instance()->Update();

	EffectMgr::Instance()->Update();

	Tutorial::Instance()->Update();

	for (auto& enemy : titleEnemy)
	{
		if (enemy->DeadTrigger())
		{
			deadNum++;
			enemy->DeadTriggerReset();
		}
	}

	if (bgmTimer < ZOOM_CAM_MOTION_TOTAL_TIME)
	{
		bgmTimer++;
		AudioMgr::Instance()->ChangeVolume(bgm, Easing::Linear(bgmTimer, ZOOM_CAM_MOTION_TOTAL_TIME, 1.0f, 0.0f));
	}

	if (!changeScene)
	{
		if (deadNum == STR_LENGTH)
		{
			changeScene = true;
			SceneMgr::Instance()->ChangeGameScene();
			bgmTimer = 0;
		}
	}
	else
	{
	}

	//PressAnyKey点滅
	if (alphaAngle < PI)
	{
		alphaAngle += PI / 40;
		pressAnyKeySprite->GetAlpha() = 1.0f - sin(alphaAngle);
		if (PI <= alphaAngle)flashTimer = 0;
	}
	else
	{
		if (nonOperateTimer == NON_OPERATE_TOTAL_TIME && flashTimer < FLASH_SPAN)
		{
			flashTimer++;
			if (flashTimer == FLASH_SPAN)
			{
				alphaAngle = 0.0f;
			}
		}
	}

	//タイトルロゴ動き
	if (titleMoveTimer < TITLE_MOVE_TOTAL_TIME)
	{
		titleMoveTimer++;

		if (titleAppear)
		{
			titleSprite->GetComponent().transform.pos.y = Easing::Ease(Out, Back, titleMoveTimer,
				TITLE_MOVE_TOTAL_TIME, -(WinApp::Instance()->GetWinCenter().y - TITLE_LOGO_OFFSET_Y), 
				(WinApp::Instance()->GetWinCenter().y - TITLE_LOGO_OFFSET_Y));
		}
		else
		{
			titleSprite->GetComponent().transform.pos.y = Easing::Ease(In, Back, titleMoveTimer,
				TITLE_MOVE_TOTAL_TIME, (WinApp::Instance()->GetWinCenter().y - TITLE_LOGO_OFFSET_Y), 
				-(WinApp::Instance()->GetWinCenter().y - TITLE_LOGO_OFFSET_Y));
		}
	}

	//無操作時間計測
	if (nonOperateTimer < NON_OPERATE_TOTAL_TIME)
	{
		nonOperateTimer++;
		if (nonOperateTimer == NON_OPERATE_TOTAL_TIME)
		{
			titleMoveTimer = 0;
			titleAppear = true;
			alphaAngle = PI / 2.0f;
			Tutorial::Instance()->Move();
		}
	}
	//操作があったら計測開始
	if (UsersInput::Instance()->OnTriggerAnyKey())
	{
		nonOperateTimer = 0;

		if (titleAppear)
		{
			titleMoveTimer = 0;
			titleAppear = false;
			Tutorial::Instance()->Move();
		}
	}

	//if (UsersInput::Instance()->OnTrigger(DIK_I))
	//{
	//	SceneMgr::Instance()->ChangeScene(0);
	//}
	//if (UsersInput::Instance()->OnTrigger(DIK_D))
	//{
	//	if (nowCam)nowCam = 0;
	//	else nowCam = 1;

	//	CameraMgr::Instance()->SetCam(nowCam);
	//	CameraMgr::Instance()->Init();
	//	printf("カメラ切り替え\n");
	//}

	if (UsersInput::Instance()->OnTrigger(DIK_ESCAPE))
	{
		SceneMgr::Instance()->GameEnd();
	}
}

void TitleScene::Draw()
{
	DebugText::Instance()->Init();

	RenderTargetMgr::Instance()->ClearDepthMapRenderTarget();

	//クリッピングクリア
	RenderTargetMgr::Instance()->ClearClippingRenderTarget();

	//ゲームオブジェクト類↓
	for (auto ptr : GameObjectInterFace::objs)
	{
		ptr->Draw();
	}

	PanelMgr::Instance()->Draw();

	BackGround::Instance()->Draw();
	//test->Draw();

	EffectMgr::Instance()->Draw3DEffect();

	//UIとかエフェクトとか(デプスステンシルのクリア)↓
	RenderTargetMgr::Instance()->ClearDepthStencil();

	EffectMgr::Instance()->DrawSpriteEffect();
	PanelMgr::Instance()->UIDraw();

	if (!changeScene)
	{
		Tutorial::Instance()->Draw();
	}

	titleSprite->Draw();
	if (nonOperateTimer == NON_OPERATE_TOTAL_TIME)
	{
		pressAnyKeySprite->Draw();
	}
}

void TitleScene::Finalize()
{
	CameraMgr::Instance()->ReleaseCam();

	PanelMgr::Instance()->Finalize();

	for (auto ptr : GameObjectInterFace::objs)
	{
		ptr->Finalize();
	}
}
