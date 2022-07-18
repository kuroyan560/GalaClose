#include "BaseCollider.h"
#include<string>
#include"MyFunc.h"

BaseCollider::~BaseCollider()
{
	MyFunc::SafetyDelete(broadPhase);
}

void BaseCollider::Update()
{
	if (object->MatWorldDirty())
	{
		ForciblyUpdate();
	}
	if (broadPhase != nullptr)
	{
		broadPhase->Update();
	}
}

void BaseCollider::SetAttribute(unsigned short Attribute)
{
	attribute = Attribute;
}

void BaseCollider::AddAttribute(CollisionAttribute Attribute)
{
	attribute |= Attribute;
}

void BaseCollider::RemoveAttribute(CollisionAttribute Attribute)
{
	attribute &= !Attribute;
}

bool BaseCollider::MatchAttribute(unsigned short Attribute)
{
	return attribute & Attribute;
}
