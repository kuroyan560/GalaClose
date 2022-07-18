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

	//両方球
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
	//全ての組み合わせについて総当りチェック
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
	//今までで最も近いコライダーを記録するためのイテレータ
	std::forward_list<CollisionInterface*>::iterator it_hit;
	//今までで最も近いコライダーの距離を記録する変数
	float distance = MaxDistance;
	//今までで最も近いコライダーとの交点を記録する変数
	XMVECTOR inter;
	//メッシュコライダーだった場合、メッシュIDを格納する変数
	int meshIndex = -1;
	//メッシュコライダーだった場合、メッシュのAttributeを格納する変数
	int meshAttribute = -1;
	//全てのコライダーと総当たり
	for (auto it = colliders.begin(); it != colliders.end(); ++it)
	{
		BaseCollider* colA = (*it)->GetCollider();

		//当たり判定をしない設定になっていたら無視
		if (!colA->CheckFlag(nullptr))continue;

		//NONEのときは全属性で判定を取る
		//指定があった属性と一致しなければとばす
		if (Attribute != COLLISION_ATTR_NONE && !colA->MatchAttribute(Attribute))
		{
			continue;
		}

		//球の場合
		if (colA->shapeType == COLLISION_SHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tmpDistance;
			XMVECTOR tmpInter;
			//当たらなければ除外
			if (!Collision::CheckRay2Sphere(Ray, *sphere, &tmpDistance, &tmpInter))continue;
			//距離が最小でなければ除外
			if (tmpDistance >= distance)continue;
			//今までで最も近いので記録を取る
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
					//当たらなければ除外
					if (!Collision::CheckRay2Sphere(Ray, *sphere, &tmpDistance, &tmpInter))continue;
				}
			}

			if (!meshCollider->CheckCollisionRay(Ray, &tmpDistance, &tmpInter,&meshIndex,&meshAttribute))continue;
			//距離が最小でなければ除外
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

	//最終的に何かに当たっていたら結果を書き込む
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
