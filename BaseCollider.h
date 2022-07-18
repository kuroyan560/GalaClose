#pragma once
#include"CollisionInterface.h"
#include"ColliderInfo.h"

class BaseCollider
{
protected:
	friend class CollisionInterface;
	friend class CollisionMgr;
	CollisionInterface* object = nullptr;
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	unsigned short attribute = COLLISION_ATTR_NONE;

	//ブロードフェーズ
	BaseCollider* broadPhase = nullptr;

	//衝突判定を受け付けるか（falseのとき、一切の当たり判定を取らない）
	bool checkFlag = true;

	int id = -1;

	BaseCollider(CollisionShapeType ShapeType, int ID)
		:shapeType(ShapeType), id(ID) {};

public:
	virtual ~BaseCollider();

	inline void OnCollision(const CollisionInfo& Info)
	{
		object->OnCollision(Info);
	}
	void Update();
	virtual void ForciblyUpdate() = 0;	//DirtyなしUpdate

	bool CheckFlag(BaseCollider* Target)
	{
		return object->HitCheckFlag(Target);
	}
	void SetAttribute(unsigned short Attribute);
	void AddAttribute(CollisionAttribute Attribute);
	void RemoveAttribute(CollisionAttribute Attribute);

	bool MatchAttribute(unsigned short Attribute);

	CollisionInterface* GetAttachObject() { return object; }
};