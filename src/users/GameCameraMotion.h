#pragma once
#include<memory>
#include<DirectXMath.h>
using namespace DirectX;
class GameCameraMotionHelper;
class GameCamera;

class GameCameraMotion
{
	GameCamera* gameCam;
	int motionTimer = 0;	//���[�V�������̎��Ԃ𑪂�^�C�}�[
	std::shared_ptr<GameCameraMotionHelper>nowMotion;	//���݃Z�b�g����Ă��郂�[�V����
	XMFLOAT3 startEye;	//���[�V�����J�n���̎��_���W
	XMFLOAT3 startTarget;	//���[�V�����J�n���̉�]��

public:
	GameCameraMotion(GameCamera* GameCamera);
	void Init();
	void Update();
	void StartMotion(const GameCameraMotionHelper& MotionData);

	bool NowMotion() { return nowMotion != nullptr; }
};

