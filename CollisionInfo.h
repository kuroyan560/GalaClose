#pragma once
#include<DirectXMath.h>

class BaseCollider;

//衝突に関する情報
struct CollisionInfo
{
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突点
	DirectX::XMVECTOR inter;
public:
	CollisionInfo(BaseCollider* Collider, const DirectX::XMVECTOR& Inter)
		:collider(Collider), inter(Inter) {};
};