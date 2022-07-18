#pragma once
/// <summary>
/// 当たり判定プリミティブ
/// </summary>

#include <DirectXMath.h>

// 球
struct Sphere
{
	// 中心座標
	DirectX::XMVECTOR center = {};
	// 半径
	float radius = 1.0f;

	DirectX::XMFLOAT3 floatCenter;
};

//平面
struct Plane
{
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0 };
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

// 法線付き三角形（時計回りが表面）
class Triangle
{
public:
	// 頂点座標3つ
	DirectX::XMVECTOR	p0;
	DirectX::XMVECTOR	p1;
	DirectX::XMVECTOR	p2;
	// 法線ベクトル
	DirectX::XMVECTOR	normal;

	// 法線の計算
	void ComputeNormal();
};

struct Ray
{
	// 始点座標
	DirectX::XMVECTOR	start = { 0,0,0,1 };
	// 方向
	DirectX::XMVECTOR	dir = { 1,0,0,0 };
};

