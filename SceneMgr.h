#pragma once
#include"Scene.h"
#include"Singleton.h"
#include<vector>
#include"src/users/ToTitleScene.h"
#include"src/users/ToGameScene.h"

class Transition;

class SceneMgr : public Singleton<SceneMgr>
{
private:
	friend void ImguiCommand_Master();

	SceneMgr();
	friend Singleton<SceneMgr>;
	std::vector<Scene*>scenes;

	int nowScene;
	int nextScene;

	bool end = false;

	Transition* sceneTransition = nullptr;
	Transition* nowSceneTransition = nullptr;

	ToTitleScene toTitleTrans;
	ToGameScene toGameTrans;

public:
	~SceneMgr();
	void Update();
	void Draw();

	bool End();	//終了条件
	void GameEnd() { end = true; }

	void ChangeScene(int SceneNum, Transition* SceneTrans = nullptr);
	void ChangeGameScene();
	void ChangeTitleScene();
	bool SceneChangeComplete();	//シーン遷移が完全に終了したか
};