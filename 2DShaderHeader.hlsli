cbuffer cbuff0 : register(b0)
{
	matrix view; // ビュー行列
	matrix proj;
	matrix world; // ワールド行列
	float3 cameraPos; // カメラ座標（ワールド座標）
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION; // ワールド座標
	float3 normal :NORMAL; // 法線
	float2 uv  :TEXCOORD; // uv値
};