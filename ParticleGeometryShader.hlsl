#include"ParticleShaderHeader.hlsli"

static const uint vnum = 4; //四角形

//センターからのオフセット
static const float4 offsetArray[vnum] =
{
	float4(-0.5f,-0.5f,0,0),	//左下
	float4(-0.5f,+0.5f,0,0),	//左上
	float4(+0.5f,-0.5f,0,0),	//右下
	float4(+0.5f,+0.5f,0,0),	//右上
};

static const float2 uvArray[vnum] =
{
	float2(0.0f,1.0f),//左下
	float2(0.0f,0.0f),//左上
	float2(1.0f,1.0f),//右下
	float2(1.0f,0.0f) //右上
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : POSITION,	//１点から四角形を生成
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	//UVのオフセット単位(一枚のパーティクルのuv最大値)
	float2 uvOffsetUint = { 1.0f / (float)imgSplitX,1.0f / (float)imgSplitY };
    if (imgSplitX == 0)
        uvOffsetUint.x = 0.0f;
    if (imgSplitY == 0)
        uvOffsetUint.y = 0.0f;
	// = 0.3333f,1.0f

	for (uint i = 0; i < vnum; i++)
	{
		//スケールを適用
		float4 offset = offsetArray[i];
		offset.xy *= input[0].scale;
		//中心からのオフセットをビルボード回転
		offset = mul(billboardMat, offset);
		//オフセット分ずらす
		element.svpos = input[0].pos + offset;
		//ビュープロジェクション変換
		element.svpos = mul(viewProjMat, element.svpos);

		element.uv = uvArray[i] * uvOffsetUint;
		element.uv += input[0].index * uvOffsetUint;

		element.color = input[0].color;
		output.Append(element);
	}
}