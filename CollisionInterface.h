#pragma once
#include<DirectXMath.h>
using namespace DirectX;
#include"ColliderInfo.h"
#include"HSM.h"
class BaseCollider;
class CollisionInfo;
//当たり判定機能をつけるためのインターフェイスクラス
class CollisionInterface
{
protected:
	BaseCollider* collider = nullptr;
public:
	CollisionInterface() {};
	virtual ~CollisionInterface();

	//コライダーのセット、CollisionMgrに判定を委託するか
	void SetCollider(BaseCollider* Collider, unsigned short Attribute = COLLISION_ATTR_NONE);

	void CommitCollisionMgr();
	void ReleaseCollisionMgr();

	//ブロードフェーズをセット
	void SetBroadPhase(BaseCollider* BroadPhaseCollider);

	//衝突判定を行うかどうか
	virtual bool HitCheckFlag(BaseCollider* Target) = 0;
	//衝突判定がtrueだったときの処理
	virtual void OnCollision(const CollisionInfo& Info) = 0;

	//適用するワールド行列のDirtyFlag
	virtual bool MatWorldDirty() = 0;
	//適用するワールド行列のゲッタ
	virtual HSM::Transform& GetTransform() = 0;

	//コライダーゲッタ
	BaseCollider* GetCollider() { return collider; }
};