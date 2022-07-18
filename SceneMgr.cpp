#include "SceneMgr.h"
#include"GameScene.h"
#include"D3D12App.h"
#include"UsersInput.h"
#include"PipelineMgr.h"
#include"DescHeap.h"
#include"DirtyFlag.h"
#include"CameraMgr.h"
#include"Object3D.h"
#include"MyFunc.h"
#include"imguiApp.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"
#include"CollisionMgr.h"
#include"TitleScene.h"
#include"RenderTargetMgr.h"
#include"Transition.h"
#include"AudioMgr.h"

SceneMgr::SceneMgr()
{
	scenes.push_back(new TitleScene());
	scenes.push_back(new GameScene());
	/*nowScene = 0;*/
	nowScene = 0;
	nextScene = -1;

	scenes[nowScene]->Init();
	DirtyFlag::AllRecord();

	sceneTransition = new Transition;
}

SceneMgr::~SceneMgr()
{
	for (int i = 0; i < scenes.size(); i++)
	{
		MyFunc::SafetyDelete(scenes[i]);
	}
	MyFunc::SafetyDelete(sceneTransition);
}

void SceneMgr::Update()
{
	DirtyFlag::AllRecord();

	AudioMgr::Instance()->Update();

	//bool debugChanged = false;
	//Master::Instance()->Update(debugChanged);

	//if(!Master::Instance()->DebugMode())
	//{
	//	ActivityEditor_3D::ActivesUpdate();
	//	ActivityEditor_2D::ActivesUpdate();
	//	ActivityEditor_1D::ActivesUpdate();
	//}

	bool flag = false;
	if (nowSceneTransition != nullptr)
	{
		flag = nowSceneTransition->Update();

		if (nowSceneTransition->Finish())
		{
			nowSceneTransition = nullptr;
		}
	}

	//シーン切り替え
	if (nextScene != -1 && flag)/* || debugChanged*/
	{
		DirtyFlag::Init();

		//CollisionMgr::Instance()->Init();
		scenes[nowScene]->Finalize();
		nowScene = nextScene;
		//Object3D::StaticInit();
		scenes[nowScene]->Init();
		//Master::Instance()->Init();
		//CameraMgr::Instance()->Init();

		Object3D::MatConstMappingAll();

		nextScene = -1;
	}

	//CollisionMgr::Instance()->UpdateAllCollisions();
	//CollisionMgr::Instance()->CheckAllCollisions();

	UsersInput::Instance()->Update();

	scenes[nowScene]->Update();
	CameraMgr::Instance()->Update();

	//if (CameraMgr::Instance()->Dirty())
	//{
	//	Object3D::MatConstMappingAll();
	//}
}

void SceneMgr::Draw()
{
	D3D12App::Instance()->PreDraw();
	RenderTargetMgr::Instance()->PreRenderTarget();
	DescHeap::SetHeaps();

	scenes[nowScene]->Draw();

	if (nowSceneTransition != nullptr)
	{
		nowSceneTransition->Draw();
	}

	//最終的な結果をフレームバッファに描きこむ
	RenderTargetMgr::Instance()->CopyResultToFrameBuff();

	//imguiApp::Instance()->CommandExcute();

	D3D12App::Instance()->PostDraw();
	PipelineMgr::Instance()->Reset();
}

bool SceneMgr::End()
{
	return end;
}

void SceneMgr::ChangeScene(int SceneNum, Transition* SceneTrans)
{
	nextScene = SceneNum;
	if (SceneTrans != nullptr)
	{
		nowSceneTransition = SceneTrans;
	}
	else
	{
		nowSceneTransition = sceneTransition;
	}
	nowSceneTransition->Start();
}

void SceneMgr::ChangeGameScene()
{
	ChangeScene(1, &toGameTrans);
}

void SceneMgr::ChangeTitleScene()
{
	ChangeScene(0, &toTitleTrans);
}

bool SceneMgr::SceneChangeComplete()
{
	if (nowSceneTransition == nullptr)return true;
	return nowSceneTransition->Finish();
}
