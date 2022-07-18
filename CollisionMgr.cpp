#include "CollisionMgr.h"
#include"BaseCollider.h"
#include"Collision.h"
#include "CollisionPrimitive.h"
#include"MeshCollider.h"
#include"CollisionInfo.h"

using namespace DirectX;

void CollisionMgr::CollisionCheck(BaseCollider* A, BaseCollider* B)
{
	if (!A->CheckFlag(B) || !B->CheckFlag(A))return;

	//������
	if (A->shapeType == COLLISION_SHAPE_SPHERE
		&& B->shapeType == COLLISION_SHAPE_SPHERE)
	{
		Sphere* sphereA = dynamic_cast<Sphere*>(A);
		Sphere* sphereB = dynamic_cast<Sphere*>(B);
		DirectX::XMVECTOR inter;
		if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter))
		{
			A->OnCollision(CollisionInfo(B, inter));
			B->OnCollision(CollisionInfo(A, inter));
		}
	}
	else if (A->shapeType == COLLISION_SHAPE_MESH
		&& B->shapeType == COLLISION_SHAPE_SPHERE)
	{
		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(A);
		Sphere* sphere = dynamic_cast<Sphere*>(B);
		DirectX::XMVECTOR inter;
		if (meshCollider->CheckCollisionSphere(*sphere, &inter))
		{
			A->OnCollision(CollisionInfo(B, inter));
			B->OnCollision(CollisionInfo(A, inter));
		}
	}
	else if (A->shapeType == COLLISION_SHAPE_SPHERE
		&& B->shapeType == COLLISION_SHAPE_MESH)
	{
		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(B);
		Sphere* sphere = dynamic_cast<Sphere*>(A);
		DirectX::XMVECTOR inter;
		if (meshCollider->CheckCollisionSphere(*sphere, &inter))
		{
			A->OnCollision(CollisionInfo(B, inter));
			B->OnCollision(CollisionInfo(A, inter));
		}
	}
}

void CollisionMgr::CheckAllCollisions()
{
	//�S�Ă̑g�ݍ��킹�ɂ��đ�����`�F�b�N
	for (auto itA = colliders.begin(); itA != colliders.end(); ++itA)
	{
		auto itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB)
		{
			BaseCollider* colA = (*itA)->GetCollider();
			BaseCollider* colB = (*itB)->GetCollider();

			CollisionCheck(colA, colB);
		}
	}
}

void CollisionMgr::UpdateAllCollisions()
{
	BaseCollider* collider;
	for (auto itr : colliders)
	{
		collider = itr->GetCollider();
		if (collider != nullptr)
		{
			collider->Update();
		}
	}
}

bool CollisionMgr::Raycast(const Ray& Ray, RaycastHit* Hitinfo, unsigned short Attribute, float MaxDistance)
{
	bool result = false;
	//���܂łōł��߂��R���C�_�[���L�^���邽�߂̃C�e���[�^
	std::forward_list<CollisionInterface*>::iterator it_hit;
	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
	float distance = MaxDistance;
	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
	XMVECTOR inter;
	//���b�V���R���C�_�[�������ꍇ�A���b�V��ID���i�[����ϐ�
	int meshIndex = -1;
	//���b�V���R���C�_�[�������ꍇ�A���b�V����Attribute���i�[����ϐ�
	int meshAttribute = -1;
	//�S�ẴR���C�_�[�Ƒ�������
	for (auto it = colliders.begin(); it != colliders.end(); ++it)
	{
		BaseCollider* colA = (*it)->GetCollider();

		//�����蔻������Ȃ��ݒ�ɂȂ��Ă����疳��
		if (!colA->CheckFlag(nullptr))continue;

		//NONE�̂Ƃ��͑S�����Ŕ�������
		//�w�肪�����������ƈ�v���Ȃ���΂Ƃ΂�
		if (Attribute != COLLISION_ATTR_NONE && !colA->MatchAttribute(Attribute))
		{
			continue;
		}

		//���̏ꍇ
		if (colA->shapeType == COLLISION_SHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tmpDistance;
			XMVECTOR tmpInter;
			//������Ȃ���Ώ��O
			if (!Collision::CheckRay2Sphere(Ray, *sphere, &tmpDistance, &tmpInter))continue;
			//�������ŏ��łȂ���Ώ��O
			if (tmpDistance >= distance)continue;
			//���܂łōł��߂��̂ŋL�^�����
			result = true;
			distance = tmpDistance;
			inter = tmpInter;
			it_hit = it;
		}
		else if (colA->shapeType == COLLISION_SHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tmpDistance;
			DirectX::XMVECTOR tmpInter;
			
			BaseCollider* broad = meshCollider->GetBroad();
			if (broad != nullptr)
			{
				if (broad->shapeType == COLLISION_SHAPE_SPHERE)
				{
					Sphere* sphere = dynamic_cast<Sphere*>(broad);
					//������Ȃ���Ώ��O
					if (!Collision::CheckRay2Sphere(Ray, *sphere, &tmpDistance, &tmpInter))continue;
				}
			}

			if (!meshCollider->CheckCollisionRay(Ray, &tmpDistance, &tmpInter,&meshIndex,&meshAttribute))continue;
			//�������ŏ��łȂ���Ώ��O
			if (tmpDistance >= distance)
			{
				meshIndex = -1;
				meshAttribute = -1;
				continue;
			}

			result = true;
			distance = tmpDistance;
			inter = tmpInter;
			it_hit = it;
		}
	}

	//�ŏI�I�ɉ����ɓ������Ă����猋�ʂ���������
	if (result && Hitinfo)
	{
		Hitinfo->distance = distance;
		Hitinfo->inter = inter;
		Hitinfo->collider = (*it_hit)->GetCollider();
		Hitinfo->object = Hitinfo->collider->object;
		Hitinfo->meshIndex = meshIndex;
		Hitinfo->meshAttribute = meshAttribute;
	}

	return result;
}
