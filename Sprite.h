#pragma once
#include<wrl.h>

#include <d3d12.h>
#include"d3dx12.h"
#include<string>

#include <DirectXMath.h>
using namespace DirectX;

#include"DirtyFlag.h"
#include"ActEditorsForSprite.h"

#include"MyFunc.h"

namespace SPRITE
{
	static const enum ROOT_PARAM
	{
		CONST_BUFF,
		TEXTURE
	};
	//頂点データ
	struct Vertex
	{
		XMFLOAT3 pos;	//xyz座標
		XMFLOAT2 uv;	//uv座標
	};
	//定数バッファデータ
	struct Color
	{
		XMFLOAT4 color = { 1,1,1,1 };
	};
	struct Transform
	{
		XMFLOAT2 pos = { 0,0 };
		XMFLOAT2 rotate = { 0,0 }; //YとZ( x を z として扱う) //度数
		XMFLOAT2 scale = { 1,1 };
		XMMATRIX mat = XMMatrixIdentity();
	};
	struct Const
	{
		Color color;
		XMMATRIX mat = XMMatrixIdentity();
	};
}

class Sprite_Component;

//頂点
class Sprite
{
protected:
	friend void ImguiCommand_Master();
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const std::string PIPELINE_KEY;
	//左下、左上、右下、右上
	static const enum { LB, LT, RB, RT };

	static std::vector<Sprite*>STATIC_SPRITES;
	static int MOUSE_IMG_ID;	//マウススプライトのID
public:
	static void CreatePipeline();
	//最初にHitしたものを返す（探索をどのIDから始めるか）
	static Sprite* HitCheckAll(XMFLOAT2 ScreenPos, int StartID);
	static void RegisterSprite(Sprite* Tmp, bool ForCorsor = false);	//クラスの静的配列に登録
	
protected:
	int id;

	//リソース関係
	static const int INDEX_NUM = 4;
	SPRITE::Vertex vertices[INDEX_NUM];
	ComPtr<ID3D12Resource> vertBuff = nullptr;	//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView;			//頂点バッファビュー

	std::string name;
	XMFLOAT2 anchorPoint;		//基準点
	XMFLOAT2 size;

	void VertMapping();
public:
	Sprite(std::string Name, int Handle, XMFLOAT2 Size,bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);
	Sprite(std::string Name, int Handle, MyDef::int2 Size,bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);

	bool flipX;	//左右反転
	bool flipY;	//上下反転
	bool draw = true;
	int texHandle;

	//スプライトの識別番号
	int GetID() { return id; }

	//指定した場所にスプライトがあるか（当たり判定）
	virtual bool HitCheck(XMFLOAT2 ScreenPos);

	//スプライトテクスチャ範囲設定
	void ChangeTexExtent(XMFLOAT2 TexPos, XMFLOAT2 TexSize);

	//サイズ変更
	void ChangeSize(XMFLOAT2 Size);

	//参照するテクスチャハンドル変更
	void ChangeTexHandle(int Handle)
	{
		texHandle = Handle;
	}

	void PreDraw();
	void Draw(Sprite_Component* Component);
};

class Sprite_Component
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> constBuff = nullptr;	//定数バッファ
	DirtyFlag transformDirty;
	DirtyFlag colorDirty;

public:
	SPRITE::Color color;
	SPRITE::Transform transform;

	Sprite_Component();

	void DirtyRegister();

	void ColorMapping();
	void MatMapping();
	void SetConstBuff();
};

class Sprite_Normal : public Sprite
{
	Sprite_Component component;
public:
	Sprite_Normal(std::string Name, int Handle, XMFLOAT2 Size,
		XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);	//カーソル用のものはスプライト選択できなようにする
	Sprite_Normal(std::string Name, int Handle, MyDef::int2 Size,
		XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);	//カーソル用のものはスプライト選択できなようにする
	~Sprite_Normal() {};

	void Draw();

	//コンポーネントのゲッタ
	Sprite_Component& GetComponent() { return component; }

	//指定した場所にスプライトがあるか（当たり判定）
	bool HitCheck(XMFLOAT2 ScreenPos)override;

	void ConstMapping();

	//アルファ値取得
	float& GetAlpha() { return component.color.color.w; }
	//色度合い補正（０～２５５）
	void ChangeColor(int R, int G, int B);
};

class Sprite_Divide :public Sprite_Normal
{
	//画像の分割数
	MyDef::int2 totalSplit;

	//分割したときの一枚のサイズ
	MyDef::float2 splitSize;

	//現在描画したい画像の番号
	MyDef::int2 split = { 0,0 };
public:
	Sprite_Divide(std::string Name, MyDef::int2 SplitNum, MyDef::int2 SplitSize, int Handle, XMFLOAT2 Size,
		XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);

	//何番の画像を描画するか
	void SelectImgNum(int X, int Y);
};