cbuffer cbuff0 : register(b0)
{
    matrix view; // ビュープロジェクション行列
    matrix proj;
    float3 cameraPos; // カメラ座標（ワールド座標）
}

struct VSOutput
{
    float4 pos : POSITION; //頂点座標
    float3 rotate : ROTATE;
    float radius : RADIUS;
    float4 color : COLOR;
    min16uint primitive : PRIMITIVE; //形状
	//min16uint … unsigned short
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //システム用頂点座標
    float3 normal : NORMAL; // 法線
    float4 color : COLOR;
    float3 ray : VECTOR; //視線ベクトル
};