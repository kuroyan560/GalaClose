#pragma once
#include"../../Transition.h"
#include<memory>

class Sprite_Normal;

class ToGameScene : public Transition
{
	std::shared_ptr<Sprite_Normal>ready;
	int timer;

public:
	ToGameScene();
	virtual void Start()override;	//�V�[���J�ڃX�^�[�g
	virtual bool Update()override;	//�V�[����؂�ւ���^�C�~���O��true��Ԃ�
	virtual void Draw()override;
};