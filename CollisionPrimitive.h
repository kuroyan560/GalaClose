#pragma once
/// <summary>
/// �����蔻��v���~�e�B�u
/// </summary>

#include <DirectXMath.h>

// ��
struct Sphere
{
	// ���S���W
	DirectX::XMVECTOR center = {};
	// ���a
	float radius = 1.0f;

	DirectX::XMFLOAT3 floatCenter;
};

//����
struct Plane
{
	// �@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0 };
	// ���_(0,0,0)����̋���
	float distance = 0.0f;
};

// �@���t���O�p�`�i���v��肪�\�ʁj
class Triangle
{
public:
	// ���_���W3��
	DirectX::XMVECTOR	p0;
	DirectX::XMVECTOR	p1;
	DirectX::XMVECTOR	p2;
	// �@���x�N�g��
	DirectX::XMVECTOR	normal;

	// �@���̌v�Z
	void ComputeNormal();
};

struct Ray
{
	// �n�_���W
	DirectX::XMVECTOR	start = { 0,0,0,1 };
	// ����
	DirectX::XMVECTOR	dir = { 1,0,0,0 };
};

