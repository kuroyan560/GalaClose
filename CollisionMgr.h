#pragma once
#include"Singleton.h"
#include<forward_list>

#include"CollisionPrimitive.h"
#include"RaycastHit.h"

#include<d3d12.h>
#include"CollisionInterface.h";

class BaseCollider;

class CollisionMgr : public Singleton<CollisionMgr>
{
private:
	CollisionMgr() {};
	friend Singleton<CollisionMgr>;

	//std::forward_list<BaseCollider*>colliders;
	std::forward_list<CollisionInterface*>colliders;

	void CollisionCheck(BaseCollider* A, BaseCollider* B);

public:
	void Init() { colliders.clear(); }
	void CheckAllCollisions();
	void UpdateAllCollisions();

	bool Raycast(const Ray&, RaycastHit* Hitinfo = nullptr, unsigned short Attribute = COLLISION_ATTR_NONE, float MaxDistance = D3D12_FLOAT32_MAX);

	//�R���C�_�[�C���^�[�t�F�C�X�o�^
	void RegisterColliderInterface(CollisionInterface* Interface)
	{
		colliders.push_front(Interface);
	}
	//�R���C�_�[�C���^�[�t�F�C�X����
	void ReleaseColliderInterface(CollisionInterface* Interface)
	{
		colliders.remove(Interface);
	}
};