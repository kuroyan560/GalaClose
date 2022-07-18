#pragma once
#include"ActivityEditor3D.h"
#include"Model_HSM.h"

//アクティビティエディタを一括管理するための仲介クラス3D(Object3D用)
class ActEditors_Obj3D
{
public:
	static const std::string EXTENSION;	//拡張子
private:
	friend void ImguiCommand_Master();
	ActivityEditor_3D* activityEditor[ACTIVE_TYPE_NUM] = { nullptr };
	std::string name;

	//失敗したらfalseを返す
	bool Load(std::string& ActEditorName, std::string* DirPass = nullptr);

public:
	ActEditors_Obj3D(std::string Name, XMFLOAT3* Pos, XMFLOAT3* Scale, XMFLOAT3* Rotation, bool Load = false, std::string* DirPass = nullptr);
	ActEditors_Obj3D(std::string Name, HSM::Transform& Transform, bool Load = false, std::string* DirPass = nullptr);
	~ActEditors_Obj3D();

	void Init();

	//初期値セット
	void InitStatus();
	//起動
	void AllOn();
	//中断
	void Interrupt();

	//.aeファイルから読み込み
	void LoadAE(std::string Name);

	//現在の状態を.aeに出力
	void SaveAE(std::string* DirPass = nullptr);

	bool Finish();

	void SetTarget(XMFLOAT3* Pos, XMFLOAT3* Scale, XMFLOAT3* Rotation);
	void SetTarget(HSM::Transform& Transform);

	std::string GetName() { return name; }
};