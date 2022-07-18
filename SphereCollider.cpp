#include "SphereCollider.h"

void SphereCollider::ForciblyUpdate()
{
	//ワールド行列から座標を抽出
	//球のメンバ変数を更新
	float3 pos = object->GetTransform().pos;
	XMVECTOR vec = { pos.x,pos.y,pos.z,1 };
	center = vec + offset;
	XMStoreFloat3(&floatCenter, center);
	Sphere::radius = SphereCollider::radius;
}