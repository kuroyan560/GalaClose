#include "SphereCollider.h"

void SphereCollider::ForciblyUpdate()
{
	//���[���h�s�񂩂���W�𒊏o
	//���̃����o�ϐ����X�V
	float3 pos = object->GetTransform().pos;
	XMVECTOR vec = { pos.x,pos.y,pos.z,1 };
	center = vec + offset;
	XMStoreFloat3(&floatCenter, center);
	Sphere::radius = SphereCollider::radius;
}