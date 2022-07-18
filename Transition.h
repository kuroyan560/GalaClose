#pragma once
class Transition
{
protected:
	bool nowTrans = false;
public:
	virtual ~Transition() {};
	virtual void Start() { nowTrans = true; }	//�V�[���J�ڃX�^�[�g
	virtual bool Update() { return true; nowTrans = false; }	//�V�[����؂�ւ���^�C�~���O��true��Ԃ�
	virtual void Draw() {};
	bool Finish() { return !nowTrans; }	//�V�[���J�ڂ����S�ɏI��������
};