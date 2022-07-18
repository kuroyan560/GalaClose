#include"PrimitiveParticleHeader.hlsli"

static const float PI = 3.14159265359f;

static const uint vnum[1] = { 36 };


//CUBE ====================================
static const int3 cubeOffset[8] =
{
    { -1, -1, -1 },
    { 1, -1, -1 },
    { -1, 1, -1 },
    { 1, 1, -1 },
    { -1, -1, 1 },
    { 1, -1, 1 },
    { -1, 1, 1 },
    { 1, 1, 1 }
};
static const int cubeOffsetIdx[vnum[0]] =
{
    0, 2, 1,    //手前１
    1, 2, 3,    //手前2
    1, 3, 5,    //右1
    5, 3, 7,    //右２
    5, 7, 4,    //奥1
    4, 7, 6,    //奥2
    4, 6, 0,    //左1
    0, 6, 2,    //左2
    2, 6, 3,    //上1
    3, 6, 7,    //上2
    0, 1, 5,    //下1
    0, 5, 4 ,   //下2
};
static const float3 cubeNormal[6] =
{
    { 0, 0, -1 },
    { 1, 0, 0 },
    { 0, 0, 1 },
    { -1, 0, 0 },
    { 0, 1, 0 },
    { 0, -1, 0 }
};

//==============================

//回転行列を求める
matrix GetRoateMat(float3 Angle)
{
    //ラジアンに直す
    Angle.x = PI / 180 * Angle.x;
    Angle.y = PI / 180 * Angle.y;
    Angle.z = PI / 180 * Angle.z;
    
    matrix matX =
    {
        1, 0, 0, 0,
        0, cos(Angle.x), -sin(Angle.x), 0,
        0, sin(Angle.x), cos(Angle.x), 0,
        0, 0, 0, 1
    };
    matrix matY =
    {
        cos(Angle.y), -sin(Angle.y), 0, 0,
        sin(Angle.y), cos(Angle.y), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    matrix matZ =
    {
        cos(Angle.z), 0, sin(Angle.z), 0,
        0, 1, 0, 0,
        -sin(Angle.z), 0, cos(Angle.z), 0,
        0, 0, 0, 1
    };

    return mul(mul(matX, matY), matZ);
}

[maxvertexcount(64)]
void main(
	point VSOutput input[1] : POSITION,		//１点からプリミティブ生成
	inout TriangleStream< GSOutput > output
)
{
    GSOutput element;
    //カラーはそのまま渡す
    element.color = input[0].color;
    
    float4 center = input[0].pos;   //中心
    matrix rotateMat = GetRoateMat(input[0].rotate); //回転角度
    float radius = input[0].radius;
    
    //形状によって頂点数が変わる
    for (uint i = 0; i < vnum[input[0].primitive]; ++i)
    {
        //CUBE
        if(input[0].primitive == 0)
        {
            float side = radius * (2 / sqrt(3)); //辺の長さ
            int index = cubeOffsetIdx[i];
            float4 pos = center;
            pos.x += cubeOffset[index].x * side;
            pos.y += cubeOffset[index].y * side;
            pos.z += cubeOffset[index].z * side;
            
            pos = center + mul(rotateMat, pos - center);
            
            index = i / 6;
            element.normal = cubeNormal[index];
            element.ray = normalize(pos.xyz - cameraPos);
            
            pos = mul(proj,mul(view, pos)); //ビュー変換
            element.svpos = pos;
        }
        
        output.Append(element);
    }
}