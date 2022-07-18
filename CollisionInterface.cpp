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
	//�R���C�_�[�̃Z�b�g
	collider = Collider;
	//�R���C�_�[�ɃI�u�W�F�N�g��ݒ�
	collider->object = this;
	//�R���C�_�[�̍X�V
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
	//�u���[�h�t�F�[�Y��ݒ�
	collider->broadPhase = BroadPhaseCollider;
	//�u���[�h�t�F�[�Y�R���C�_�[�ɃI�u�W�F�N�g��ݒ�
	BroadPhaseCollider->object = collider->object;
	//�u���[�h�t�F�[�Y�R���C�_�[�̍X�V
	BroadPhaseCollider->ForciblyUpdate();
}

