#pragma once
#include<DirectXMath.h>
#include<memory>
#include"GameObjectInterFace.h"
#include"Panel.h"
#include"../../Model_HSM.h"
#include"EnemyGenerateHelper.h"
#include"EnemyExplosion.h"

class Enemy : public GameObjectInterFace
{
public:
	static int ENEMY_KILL_SE;
	static void LoadAudios();
	Enemy();

	/// <summary>
	/// �������A�X�|�[��������ۂɎg��
	/// </summary>
	/// <param name="isClockWise">����</param>
	/// <param name="posAngle">���݈ʒu</param>
	/// <param name="panelIndex">���ݏ�������p�l��</param>
	/// <param name="layer">���݂���K�w</param>
	/// <param name="panelType">�F�Ȃǂ̑���</param>
	/// <param name="speedRate">���x�{��</param>
	/// 
	virtual void UniqueInit() {};
	virtual void UniqueUpdate();
	virtual void UniqueFinalize() {};
	void Init()override;
	void Update()override;
	void Finalize()override;
	virtual void Draw()override;

	//�K�w��������(�K�w���[���̎��ɌĂяo���Ǝ���)
	void Down(const int& PushedNum);


	//�����t���O��true�ɂ���
	virtual void Spawn(const EnemyGenerateHelper& InitData);

	//����
	virtual void Dead();

	void Collision();

	//�L�����N�^�[�̑����Ԃ�
	char GetType();

	//���ݏ�������p�l���̔ԍ���Ԃ�
	int GetPanelIndex();

	const int &Getlayer();

	const float &GetAngle();

	Model_HSM_Component& GetComponent() { return component; }
	const bool& GetIsAlive() { return isAlive; }

	std::weak_ptr<Panel> GetPanel() { return panel; }
public://�f�o�b�O�p�̐��
	//void SetLayer(int num);
	void SetLayer(int LayerNum);

	//void SetType(char num);
	const ENEMY_TYPE& GetEnemyType() { return type; }

private://�萔

	////1��
	//static const float baseSpeed;
	////360��
	//static const float round;
	////���C���[���Ƃ̑傫��
	//static const float offsetLength;

private://�ÓI�����o�ϐ�
	static float baseAngle;
protected://�����o�ϐ�

	ENEMY_TYPE type = PORN;

	EnemyExplosion enemyExplosion;
	//�ǂ�����肩
	bool isClockWise;

	//�X�e�[�W�̒��S����x�̌����ɂ��邩
	float posAngle;

	//�ǂ̌����̃p�l���ɂ��邩
	std::weak_ptr<Panel> panel;

	//�ǂ̊K�w�ɂ��邩
	//�K�w�̔ԍ�
	float layerNum;

	//�ړ����x
	float moveSpeed;

	//�����Ă��邩
	bool isAlive;

	//���݂̎����̑���
	//char type;

	//DirectX::XMFLOAT3 drawPos;
	Model_HSM_Component component;

	int bodyEmitTimer;
	bool bodyEmitFlg = true;
	bool beDrained = false;

	//�X�|�[�����o
	int spawnTimer;
	float defaultScale;

	//��]�ړ����ɋ��ڂɐڐG�������̔���
	bool IsOutRange(float startAngle, float EndAngle);

	//���΂��������ɌĂяo��
	void Reverce();

	//����̃p�l����ڎw��
	void panelVisit(const int queenNum);

	void CheckQueen();


	/// <summary>
	/// �ǂ��炪�߂������p�����肷��
	/// </summary>
	/// <param name="startAngle">���v��葤��</param>
	/// <param name="endAngle">�����v��葤</param>
	/// <returns>���v���Ȃ�start���߂�������true����ȊO�Ȃ�false</returns>

	//��_�Ԃ̋����̒Z�����̃A���O�����Ƃ�
	float angleLength(float a, float b);
};