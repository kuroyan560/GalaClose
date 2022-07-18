#pragma once
#include"../../Transition.h"
#include<memory>

class Sprite_Normal;

class ToTitleScene : public Transition
{
	std::shared_ptr<Sprite_Normal>pinkScreen;
	int timer;
	static const enum { APPEAR, WAIT, DISAPPEAR };
	int flag;

public:
	ToTitleScene();
	void Start()override;	//�V�[���J�ڃX�^�[�g
	bool Update()override;	//�V�[����؂�ւ���^�C�~���O��true��Ԃ�
	void Draw()override;
};

