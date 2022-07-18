#include"CollisionInterface.h"
#include"MyFunc.h"
#include"BaseCollider.h"
#include"CollisionMgr.h"

CollisionInterface::~CollisionInterface()
{
	MyFunc::SafetyDelete(collider);
}

void CollisionInterface::SetCollider(BaseCollider* Collider, unsigned short Attribute)
{
	//コライダーのセット
	collider = Collider;
	//コライダーにオブジェクトを設定
	collider->object = this;
	//コライダーの更新
	collider->ForciblyUpdate();

	collider->SetAttribute(Attribute);
}

void CollisionInterface::CommitCollisionMgr()
{
	CollisionMgr::Instance()->RegisterColliderInterface(this);
}

void CollisionInterface::ReleaseCollisionMgr()
{
	CollisionMgr::Instance()->ReleaseColliderInterface(this);
}

void CollisionInterface::SetBroadPhase(BaseCollider* BroadPhaseCollider)
{
	//ブロードフェーズを設定
	collider->broadPhase = BroadPhaseCollider;
	//ブロードフェーズコライダーにオブジェクトを設定
	BroadPhaseCollider->object = collider->object;
	//ブロードフェーズコライダーの更新
	BroadPhaseCollider->ForciblyUpdate();
}

