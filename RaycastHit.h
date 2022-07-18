#pragma once
#include"BaseCollider.h"
#include<DirectXMath.h>

//���C�L���X�g�ɂ����𓾂�ׂ̍\����
struct RaycastHit
{
	//�Փˑ���̃I�u�W�F�N�g
	CollisionInterface* object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˓_
	DirectX::XMVECTOR inter;
	//�Փ˓_�܂ł̋���
	float distance = 0.0f;
	//���b�V���R���C�_�[�̏ꍇ�A���b�V��Index
	int meshIndex = -1;
	//���b�V���R���C�_�[�̏ꍇ�A���b�V��Attribute
	int meshAttribute = -1;
};