//パーティクル一枚 → 頂点１→ ジオメトリシェーダ
//一塊のパーティクル共通
cbuffer cbuff0 : register(b0)
{
	matrix viewProjMat; // ビュープロジェクション行列
	matrix billboardMat; // ビルボード行列

	/*
		画像を乱数で変える場合、１枚の画像に、描画される複数のパーティクルが描かれている
		このときの画像の分割数（パーティクルの画像サイズは同じにしないと見切れる）
	*/
	int imgSplitX;
	int imgSplitY;
};

struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
	float2 scale : SCALE;
	//min16uint2 … unsigned short
	min16uint2 index : INDEX;	//描画したいパーティクルの位置番号（０～分割数）
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float2 uv : TEXCOORD;	//uv値
	float4 color : COLOR;	//RGB … 色調補正、A … 単純な不透明度
};