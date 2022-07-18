cbuffer cbuff0 : register(b0)
{
	float4 masterColor;	//RGBA(0~1)
	bool shadow;
    float flash;
}

cbuffer cbuff1 : register(b1)
{
	float4 diffuse;
	float3 specular;
	float specular_c;
	float3 ambient;
	float alpha;
}

cbuffer cbuff2 : register(b2)
{
	matrix view; // ビュープロジェクション行列
	matrix proj;
	float3 cameraPos; // カメラ座標（ワールド座標）
}

cbuffer cbuff3 : register(b3)
{
	matrix world; // ワールド行列
}

cbuffer cbuff4 : register(b4)
{
    matrix bones[256];	//ボーン行列
}

//cbuffer cbuff4 : register(b4)
//{
//	float3 dirDirection;
//	float3 dirColor;
//	float3 ptPosition;
//	float3 ptColor;
//	float ptRange;
//	float3 ambientLight;
//}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 worldpos : POSITION;
	float3 normal :NORMAL0; // 法線
	float4 vnormal : NORMAL1;	//ビュー変換後の法線ベクトル
	float2 uv  :TEXCOORD; // uv値
	float3 ray : VECTOR;	//視線ベクトル

	//カメラ空間でのZ
	float depthInView : CAM_Z;
};

struct PSOutput
{
	float4 color : SV_Target0;	//レンダーターゲット０に描き込み
	float depth : SV_Target1;	//レンダーターゲット１に描き込み
    float clip : SV_Target2;
};