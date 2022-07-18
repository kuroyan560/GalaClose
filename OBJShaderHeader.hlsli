cbuffer cbuff0 : register(b0)
{
	matrix view; // ビュー行列
	matrix proj;
	matrix world; // ワールド行列
	float3 cameraPos; // カメラ座標（ワールド座標）
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0); // アンビエント係数
	float3 m_diffuse  : packoffset(c1); // ディフューズ係数
	float3 m_specular : packoffset(c2); // スペキュラー係数
	float m_alpha : packoffset(c2.w);	// アルファ
}

// 平行光源の数
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;    // ライトへの方向の単位ベクトル
	float3 lightcolor;    // ライトの色(RGB)
	uint actives;
};

//cbuffer cbuff2 : register(b2)
//{
//	float3 ambientColor;
//	DirLight dirLights[DIRLIGHT_NUM];
//}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION; // ワールド座標
	float3 normal :NORMAL; // 法線
	float2 uv  :TEXCOORD; // uv値
	float3 ray : VECTOR;	//視線ベクトル
	
	//カメラ空間でのZ
    float depthInView : CAM_Z;
};

struct PSOutput
{
    float4 color : SV_Target0; //レンダーターゲット０に描き込み
    float depth : SV_Target1; //レンダーターゲット１に描き込み
    float clip : SV_Target2;
};
