#pragma once
#include<DirectXMath.h>
using namespace DirectX;
#include"ColliderInfo.h"
#include"HSM.h"
class BaseCollider;
class CollisionInfo;
//�����蔻��@�\�����邽�߂̃C���^�[�t�F�C�X�N���X
class CollisionInterface
{
protected:
	BaseCollider* collider = nullptr;
public:
	CollisionInterface() {};
	virtual ~CollisionInterface();

	//�R���C�_�[�̃Z�b�g�ACollisionMgr�ɔ�����ϑ����邩
	void SetCollider(BaseCollider* Collider, unsigned short Attribute = COLLISION_ATTR_NONE);

	void CommitCollisionMgr();
	void ReleaseCollisionMgr();

	//�u���[�h�t�F�[�Y���Z�b�g
	void SetBroadPhase(BaseCollider* BroadPhaseCollider);

	//�Փ˔�����s�����ǂ���
	virtual bool HitCheckFlag(BaseCollider* Target) = 0;
	//�Փ˔��肪true�������Ƃ��̏���
	virtual void OnCollision(const CollisionInfo& Info) = 0;

	//�K�p���郏�[���h�s���DirtyFlag
	virtual bool MatWorldDirty() = 0;
	//�K�p���郏�[���h�s��̃Q�b�^
	virtual HSM::Transform& GetTransform() = 0;

	//�R���C�_�[�Q�b�^
	BaseCollider* GetCollider() { return collider; }
};