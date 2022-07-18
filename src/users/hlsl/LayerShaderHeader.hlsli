static const float ALPHA = 0.2f;
//static const float3 DEFAULT_COLOR = { (float) 0x53 / (float) 0xff, (float) 0x12 / (float) 0xff, (float) 0x90 / (float) 0xff };
//static const float3 ANOTHER_COLOR = { (float) 0x80 / (float) 0xff, (float) 0x27 / (float) 0xff, (float) 0xb9 / (float) 0xff };
static const float3 DEFAULT_COLOR = { 0.5f, 0.6f, 0.8f };
static const float3 ANOTHER_COLOR = { 0.1f, 0.2f, 0.4f };
static const float3 OUTLINE_COLOR = { 0.6f, 0.7f, 0.9f };

static const float LAYER_OFFSET_RADIUS = 8.0f; //レイヤー半径（幅）

static const float LAYER_OUTLINE_OFFSET_THICKNESS = 0.3f;

cbuffer cbuff0 : register(b0)
{
    uint color;
    float pushedRate;
}

cbuffer cbuff1 : register(b1)
{
    matrix view; // ビュープロジェクション行列
    matrix proj;
    float3 cameraPos; // カメラ座標（ワールド座標）
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 pos : POSITION; // システム用頂点座標
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 ray : VECTOR; //視線ベクトル
    float depthInView : CAM_Z; //カメラ空間でのZ
};

struct PSOutput
{
    float4 color : SV_Target0; //レンダーターゲット０に描き込み
    float depth : SV_Target1; //レンダーターゲット１に描き込み
    float clip : SV_Target2;
};