#include "CameraMgr.h"
#include"UsersInput.h"
#include"MyFunc.h"
#include"Object3D.h"
#include"WinApp.h"
#include"GameCamera.h"

#pragma region �f�o�b�O�J����
const XMFLOAT3 CameraMgr::DebugCamera::INIT_POS = { 0,10,-10 };
CameraMgr::DebugCamera::DebugCamera() : Camera("DebugCamera")
{
	target = { INIT_POS.x,INIT_POS.y,0 };
	cameraInfo.eye = { target.x,target.y,-distance };

	//��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)WinApp::WIN_WIDTH;
	scaleY = 1.0f / (float)WinApp::WIN_HEIGHT;

	UpdateViewMat();
}
void CameraMgr::DebugCamera::Update()
{
	bool moveDirty = false;
	float angleX = 0;
	float angleY = 0;

	//�}�E�X�̓��͂��擾
	UsersInput::MouseMove mouseMove = UsersInput::Instance()->GetMouseMove();

	//�}�E�X���N���b�N�ŃJ������]
	if (UsersInput::Instance()->Input(UsersInput::MOUSE::RIGHT))
	{
		float dy = mouseMove.IX * scaleY;
		float dx = mouseMove.IY * scaleX;

		angleX = -dx * XM_PI;
		angleY = -dy * XM_PI;
		rotate.x += XMConvertToDegrees(-angleX);
		rotate.y += XMConvertToDegrees(-angleY);
		moveDirty = true;
	}

	//�}�E�X���N���b�N�ŃJ�������s�ړ�
	if (UsersInput::Instance()->Input(UsersInput::MOUSE::CENTER))
	{
		float dx = mouseMove.IX / 100.0f;
		float dy = mouseMove.IY / 100.0f;

		XMVECTOR move = { -dx,+dy,0,0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		moveDirty = true;
	}

	//�z�C�[�����͂ŋ�����ύX
	if (mouseMove.IZ != 0)
	{
		distance -= mouseMove.IZ / 100.0f;
		distance = max(distance, 1.0f);
		moveDirty = true;
	}

	if (moveDirty)
	{
		//�ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		SetPos({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
		SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
	}

	if (dirty.Dirty())
	{
		UpdateViewMat();
	}
}
#pragma endregion

CameraMgr::CameraMgr()
{
	cameras.push_back(&debugCam);
	nowCameraNum = DEBUG_CAM_NUM;
}

CameraMgr::~CameraMgr()
{
}

void CameraMgr::SetCam(int CameraNum)
{
	if (CameraNum < 0 || cameras.size() - 1 < CameraNum)
	{
		assert(0);
		CameraNum = DEBUG_CAM_NUM;
	}

	nowCameraNum = CameraNum;
}

void CameraMgr::Init()
{
	cameras[nowCameraNum]->Init();
}

void CameraMgr::SetCamConstBuff(int CamRootParam)
{
	cameras[nowCameraNum]->SetConstBuff(CamRootParam);
}

void CameraMgr::Update()
{
	cameras[nowCameraNum]->Update();
}

bool CameraMgr::Dirty()
{
	return (cameras[nowCameraNum]->Dirty());
}
