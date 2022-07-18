#pragma once
#include"BaseCollider.h"
#include<DirectXMath.h>

//レイキャストによる情報を得る為の構造体
struct RaycastHit
{
	//衝突相手のオブジェクト
	CollisionInterface* object = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突点
	DirectX::XMVECTOR inter;
	//衝突点までの距離
	float distance = 0.0f;
	//メッシュコライダーの場合、メッシュIndex
	int meshIndex = -1;
	//メッシュコライダーの場合、メッシュAttribute
	int meshAttribute = -1;
};