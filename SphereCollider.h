#pragma once
#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include<DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private:
	using XMVECTOR = DirectX::XMVECTOR;
public:

private:
	//オブジェクト中心からのオフセット
	XMVECTOR offset;
	//半径
	float radius;

	void ForciblyUpdate()override;

public:
	SphereCollider(float Radius, XMVECTOR Offset = { 0,0,0,0 },int ID = -1)
		:BaseCollider(COLLISION_SHAPE_SPHERE, ID), offset(Offset), radius(Radius) {}

	float& GetRadius() { return radius; }
};