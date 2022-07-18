#pragma once
#include"Singleton.h"
#include"Object3D.h"
#include"ActivityEditor3D.h"
#include"ActivityEditor2D.h"
#include"ActivityEditor1D.h"
#include"Sprite.h"

class Master : public Singleton<Master>
{
private:
	Master();
	friend Singleton<Master>;
public:

private:
	bool debugMode = false;

	//現在選択中の３Dオブジェの番号
	int selectObjNum = -1;
	//現在選択中のスプライトのポインタ
	int selectSpriteNum = -1;

	bool changedFlag = false;

	bool preview = false;
	int debugFrame = 0;
	bool activeChanged = false;

	bool sceneChanged = false;

	ActivityEditor_3D* editor_3d = nullptr;
	ActivityEditor_2D* editor_2d = nullptr;
	ActivityEditor_1D* editor_1d = nullptr;
	int nowSelectEditorMode = 0;	//POS,SCALE,ROTATION
	int nowSelectActive = 0;

	XMFLOAT3 start;
	XMFLOAT3 end;
	XMFLOAT2 p1;
	XMFLOAT2 p2;

	//アクティビティエディタのクリエイト＆ロードの名前入力に使用するデータ
	static const int MAX_AE_CHAR_COUNT = 256;
	char aeBuff[MAX_AE_CHAR_COUNT];	//名前の文字列バッファ

	void Set(bool Changed, bool& Flag);
	friend void ImguiCommand_Master();

public:
	void Init();
	void Update(bool& DebugChanged);

	bool DebugMode()const { return debugMode; }
};