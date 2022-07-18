#pragma once
#include"ActivityEditor2D.h"
#include"ActivityEditor1D.h"
#include<string>

//アクティビティエディタを一括管理するための仲介クラス(Sprite用)
class ActEditors_Sprite
{
public:
	static const std::string EXTENSION;	//拡張子

private:
	friend void ImguiCommand_Master();
	ActivityEditor_2D* actEditor_pos = nullptr;
	ActivityEditor_2D* actEditor_scale = nullptr;
	ActivityEditor_1D* actEditor_rotate = nullptr;
	std::string name;

	//失敗したらfalseを返す
	bool Load(std::string& ActEditorName);

public:
	ActEditors_Sprite(std::string Name, XMFLOAT2* Pos, XMFLOAT2* Scale, float* Rotation);
	~ActEditors_Sprite();

	//初期値セット
	void InitStatus();
	//起動
	void AllOn();
	//中断
	void Interrupt();

	ActivityEditor_2D* GetEditor2D(int EditorType)
	{
		if (EditorType == ACTIVE_POS)return actEditor_pos;
		else if (EditorType == ACTIVE_SCALE)return actEditor_scale;
		else return false;
	}

	//.aeファイルから読み込み
	void LoadAE(std::string Name);

	//現在の状態を.aeに出力
	void SaveAE();

	//名前のゲッタ
	std::string GetName() { return name; }
};

