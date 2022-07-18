#pragma once
#include<DirectXMath.h>

class BaseCollider;

//�Փ˂Ɋւ�����
struct CollisionInfo
{
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
public:
	CollisionInfo(BaseCollider* Collider, const DirectX::XMVECTOR& Inter)
		:collider(Collider), inter(Inter) {};
};