#pragma once
#include"Object3D.h"
#include<string>

class Object2D : public Object3D
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct Vertex
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	static const std::string PIPELINE_KEY;

public:
	static void CreatePipeline();

private:
	static const int VERTEX_COUNT = 4;
	static const enum { LB, RB, LT, RT };

	DirtyFlag sizeDirty;

	//頂点データ配列
	Vertex vertices[VERTEX_COUNT];
	D3D12_VERTEX_BUFFER_VIEW vbView;	//頂点バッファビュー
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	int texHandle;

	XMFLOAT2 anchorPoint;		//基準点

	void VertMapping();

public:
	Object2D(string Name, XMFLOAT2 Size, XMFLOAT3 Pos, int TexHandle, XMFLOAT2 AnchorPoint, bool BillBoard = false, bool BillBoardY = false);
	XMFLOAT2 size;

	virtual void UniqueInit()override {};
	virtual void UniqueUpdate()override {};
	virtual void UniqueFinalize()override {};
	void Draw()override;

	//スプライトテクスチャ範囲設定
	void ChangeTexExtent(XMFLOAT2 TexPos, XMFLOAT2 TexSize);
};

class Object2D_Divide : public Object2D
{
	//画像の分割数
	MyDef::int2 totalSplit;

	//分割したときの一枚のサイズ
	MyDef::float2 splitSize;

	//現在描画したい画像の番号
	MyDef::int2 split = { 0,0 };
public:
	Object2D_Divide(std::string Name, MyDef::int2 SplitNum, MyDef::int2 SplitSize, int TexHandle, XMFLOAT2 Size,
		XMFLOAT3 Pos,XMFLOAT2 AnchorPoint, bool BillBoard = false, bool BillBoardY = false);

	//何番の画像を描画するか
	void SelectImgNum(int X, int Y);
};