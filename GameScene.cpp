#include "GameScene.h"
#include"CameraMgr.h"
#include"GameCamera.h"
#include"SRVDescMgr.h"
#include"WinApp.h"
#include"MyFunc.h"
#include"UsersInput.h"
#include"AudioMgr.h"
#include"EffectMgr.h"
#include"Master.h"
#include"UIMgr.h"
#include"HowToOperate.h"
#include"RenderTargetMgr.h"

#include"PmxLoader.h"
#include"FbxLoader.h"

#include"Action.h"

#include"GameObjectInterFace.h"
#include"FixedCamera.h"
#include"Constants.h"

#include"DebugText.h"
#include"src/users/PanelMgr.h"

#include"src/users/EnemyMgr.h"

#include"SRVDescMgr.h"
#include"src/users/EnemyGenerate.h"

#include"SceneMgr.h"
#include"AudioMgr.h"

#include"src/users/GameOver.h"
#include"src/users/Score.h"
#include"src/users/ScoreLevel.h"
#include"src/users/BackGround.h"
#include"src/users/Tutorial.h"
#include"src/users/GameInterrupt.h"

GameScene::GameScene()
	:Scene("GameScene")
{
	bgmHandle = AudioMgr::Instance()->LoadAudio("resource/sound/bgm.wav");
	EnemyMgr::Instance();
	Player::Instance();
	//Object2D(string Name, XMFLOAT2 Size, XMFLOAT3 Pos, int TexHandle, XMFLOAT2 AnchorPoint, bool BillBoard = false, bool BillBoardY = false);
	test = new Object2D("test", { 32,32 }, { 0,0,0 }, SRVDescMgr::Instance()->CreateSRV("resource/img/surface.png"), { 0.5,0.5 }, true);
	//EnemyGenerate::Instance()->LoadFile("resource/stage/test.txt");
}

GameScene::~GameScene()
{
	delete test;
}

void GameScene::Init()
{
	Object3D::InitAll();
	ActivityEditor_3D::ActivesInit();
	ActivityEditor_2D::ActivesInit();
	ActivityEditor_1D::ActivesInit();
	//CameraMgr::Instance()->RegisterCam(&cam);

	EffectMgr::Instance()->Init();

	UIMgr::Instance()->Init();

	CameraMgr::Instance()->RegisterCam(GameCamera::Instance());
	CameraMgr::Instance()->SetCam(nowCam);
	//CameraMgr::Instance()->Init();

	AudioMgr::Instance()->PlayWave(bgmHandle, true);

	if (EnemyGenerate::Instance()->load)
	{
		EnemyGenerate::Instance()->PanelsInit();
		for (auto ptr : GameObjectInterFace::objs)
		{
			ptr->Init();
		}
		EnemyGenerate::Instance()->EnemysInit();
		ScoreLevel::Instance()->Init(nullptr);
	}
	else
	{
		/*PanelMgr::Instance()->Init(panelNum, layerNum, colorDifference, true);*/
		PanelMgr::Instance()->Init(4, 1, true, true);

		for (auto ptr : GameObjectInterFace::objs)
		{
			ptr->Init();
		}

		//EnemyMgr::Instance()->RandGenerate(0);
		ScoreLevel::Instance()->Init(Score::Instance()->GetScorePtr());
	}

	GameCamera::Instance()->ReturnToDefault();

	GameOver::Instance()->Init();
	Score::Instance()->Init();
	BackGround::Instance()->Init();

	Tutorial::Instance()->Init(false);

	GameInterrupt::Instance()->Init();
}

void GameScene::Update()
{
	if (!GameInterrupt::Instance()->GetOn())
	{
		//if (UsersInput::Instance()->OnTrigger(DIK_I))
		//{
		//	EnemyGenerate::Instance()->load = false;
		//	SceneMgr::Instance()->ChangeScene(1);
		//}
		//for (int i = 0; i < 8; ++i)
		//{
		//	if (UsersInput::Instance()->OnTrigger(DIK_1 + i))
		//	{
		//		std::string str = "resource/stage/" + to_string(i + 1) + ".txt";
		//		EnemyGenerate::Instance()->LoadFile(str.c_str());
		//		SceneMgr::Instance()->ChangeScene(1);
		//	}
		//}

		//ゲームオーバー演出中だんだんスローに
		if (!GameOver::Instance()->Slow())
		{
			HowToOperate::Update(true);

			Action::AllActionUpdate();

			ActivityEditor_3D::ActivesUpdate();
			ActivityEditor_2D::ActivesUpdate();
			ActivityEditor_1D::ActivesUpdate();

			EnemyGenerate::Instance()->scoreModeGenerate();

			for (auto ptr : GameObjectInterFace::objs)
			{
				ptr->Update();
			}

			BackGround::Instance()->Update();
			PanelMgr::Instance()->Update();

			EffectMgr::Instance()->Update();
		}

		UIMgr::Instance()->Update();
		Score::Instance()->Update();
		ScoreLevel::Instance()->Update();

		GameOver::Instance()->Update();

		Tutorial::Instance()->Update();

		//デバッグ用
		//デバッグカメラ切り替え
		/*if (UsersInput::Instance()->OnTrigger(DIK_D))
		{
			if (nowCam)nowCam = 0;
			else nowCam = 1;

			CameraMgr::Instance()->SetCam(nowCam);
			CameraMgr::Instance()->Init();
			printf("カメラ切り替え\n");
		}
		if (UsersInput::Instance()->OnTrigger(DIK_S))
		{
			PanelMgr::Instance()->AddPanelAndLayerReset();
		}
		if (UsersInput::Instance()->Input(DIK_U))
		{
			Score::Instance()->Add();
		}
		if (UsersInput::Instance()->OnTrigger(DIK_G))
		{
			GameOver::Instance()->Launch({ 0.0f,0.0f,0.0f });
		}*/
	}

	//PAUSE
	if (!GameOver::Instance()->Flag() && !GameInterrupt::Instance()->Restart() && UsersInput::Instance()->OnTrigger(DIK_ESCAPE))
	{
		GameInterrupt::Instance()->OnOff();
	}
	GameInterrupt::Instance()->Update();
}

void GameScene::Draw()
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

	if (!GameInterrupt::Instance()->GetOn() && !GameOver::Instance()->Flag())
	{
		ScoreLevel::Instance()->DrawObj();
	}

	PanelMgr::Instance()->Draw();

	BackGround::Instance()->Draw();
	//test->Draw();

	EffectMgr::Instance()->Draw3DEffect();

	//UIとかエフェクトとか(デプスステンシルのクリア)↓
	RenderTargetMgr::Instance()->ClearDepthStencil();

	EffectMgr::Instance()->DrawSpriteEffect();
	PanelMgr::Instance()->UIDraw();

	if (!GameInterrupt::Instance()->GetOn() && !GameOver::Instance()->Flag())
	{
		UIMgr::Instance()->Draw();
		Score::Instance()->Draw();
		ScoreLevel::Instance()->DrawUI();
		Tutorial::Instance()->Draw();
	}

	GameInterrupt::Instance()->Draw();
	GameOver::Instance()->Draw();
}

void GameScene::Finalize()
{
	CameraMgr::Instance()->ReleaseCam();
	for (auto ptr : GameObjectInterFace::objs)
	{
		ptr->Finalize();
	}
	PanelMgr::Instance()->Finalize();

	ScoreLevel::Instance()->Finalize();

	AudioMgr::Instance()->StopWave(bgmHandle);

	GameOver::Instance()->Finalize();
}
