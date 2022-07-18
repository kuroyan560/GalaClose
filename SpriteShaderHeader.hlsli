//スプライト用

cbuffer cbuff0 : register(b0)
{
	float4 color;	//色(RGBA)
	matrix mat;		//３Ｄ変換行列
};

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float2 uv : TEXCOORD;	//uv値
};

struct PSOutput
{
    float4 color : SV_Target0; //レンダーターゲット０に描き込み
    float depth : SV_Target1; //レンダーターゲット１に描き込み
    float clip : SV_Target2;
};