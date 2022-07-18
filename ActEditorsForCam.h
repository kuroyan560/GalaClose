#pragma once
#include"ActivityEditor3D.h"
#include"ActivityEditor2D.h"
class GameCamera;

class ActEditorsForCam
{
public:
	static const std::string EXTENSION;	//拡張子
	static const std::string DIRECTORY_PATH;
private:
	friend void ImguiCommand_Master();
	friend void ImguiCommand_StageMgr();
	friend void ImguiCommand_Camera();
	static const enum ACT_EDITOR_TYPE { EYE, TARGET };
	ActivityEditor_3D* actEditors[2] = { nullptr };
	std::string name;

	//失敗したらfalseを返す
	//bool Load(std::string& ActEditorName);

public:
	ActEditorsForCam(std::string Name, GameCamera* GameCam/*, bool Load = false*/);
	~ActEditorsForCam();

	void Init();
	void Update();

	//初期値セット
	void InitStatus();
	//起動
	void AllOn();
	//中断
	void Interrupt();

	////.aeファイルから読み込み
	//void LoadAE(std::string Name);

	////現在の状態を.aeに出力
	//void SaveAE();

	bool Finish();

	void SetTarget(XMFLOAT3* Eye, XMFLOAT3* Target);

	std::string GetName() { return name; }

	//総合でどれくらいのフレームがかかるか
	//int GetAllFrame();

	//情報が存在するか
	bool Empty();
};