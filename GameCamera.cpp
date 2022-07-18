#include "GameCamera.h"
#include"CameraMgr.h"

#include"imguiApp.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"

#include"UsersInput.h"

#include"MyFunc.h"

#include"ActEditorsForCam.h"
#include <Constants.h>

#include"src/users/PanelMgr.h"

#include"src/users/Constants.h"

//void GameCamera::UpdateViewMat()
//{
//	//target.x = cameraInfo.eye.x;
//	//target.y = cameraInfo.eye.y;
//	//target.z = cameraInfo.eye.z + DISTANCE;
//
//	//while(rotate.x < 0)rotate.x += 360;
//	//while(360 < rotate.x)rotate.x -= 360;
//	////�����_�̌����ς�
//	//if (0 <= rotate.x && rotate.x < 180)
//	//{
//	//	target.x += DISTANCE * sin(XMConvertToRadians(rotate.x));
//	//}
//	//else
//	//{
//	//	target.x += DISTANCE * sin(XMConvertToRadians(-rotate.x));
//	//}
//	//while (rotate.y < 0)rotate.y += 360;
//	//while (360 < rotate.y)rotate.y -= 360;
//	//if (0 <= rotate.y && rotate.y < 180)
//	//{
//	//	target.y += DISTANCE * sin(XMConvertToRadians(-rotate.y));
//	//}
//	//else
//	//{
//	//	target.y += DISTANCE * sin(XMConvertToRadians(rotate.y));
//	//}
//
//	float3 targetOffset = { 0,0,(float)DISTANCE };
//	XMVECTOR targetPosition = XMLoadFloat3(&targetOffset);
//	XMMATRIX test = XMMatrixIdentity();
//	test *= XMMatrixRotationX(XMConvertToRadians(rotate.x));
//	test *= XMMatrixRotationY(XMConvertToRadians(rotate.y));
//	targetPosition = XMVector3Transform(targetPosition, test);
//	test = XMMatrixTranslation(cameraInfo.eye.x + shakeOffset.x, cameraInfo.eye.y + shakeOffset.y, cameraInfo.eye.z + shakeOffset.z);
//	targetPosition = XMVector3Transform(targetPosition, test);
//
//	//���_���W
//	XMFLOAT3 tmp = MyFunc::AddXMFLOAT3(cameraInfo.eye, shakeOffset);
//	XMVECTOR eyePosition = XMLoadFloat3(&tmp);
//	//�����_���W
//	//tmp = MyFunc::AddXMFLOAT3(target, shakeOffset);
//	//XMVECTOR targetPosition = XMLoadFloat3(&tmp);
//	//�i���́j�����
//	XMVECTOR upVector = XMLoadFloat3(&up);
//
//	//�J����Z���i���������j
//	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
//
//	//�O�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
//	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
//	assert(!XMVector3IsInfinite(cameraAxisZ));
//	assert(!XMVector3Equal(upVector, XMVectorZero()));
//	assert(!XMVector3IsInfinite(upVector));
//
//	//�x�N�g���𐳋K��
//	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
//
//	//�J������X���i�E�����j
//	XMVECTOR cameraAxisX;
//	//X���͏������Z���̊O�ςŋ��܂�
//	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
//	//�x�N�g���𐳋K��
//	cameraAxisX = XMVector3Normalize(cameraAxisX);
//
//	//�J������Y���i������j
//	XMVECTOR cameraAxisY;
//	//Y����Z����X���̊O�ςŋ��܂�
//	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
//
//	//�J������]�s��
//	XMMATRIX matCameraRot;
//	//�J�������W�n�����[���h���W�n�̕ϊ��s��
//	matCameraRot.r[0] = cameraAxisX;
//	matCameraRot.r[1] = cameraAxisY;
//	matCameraRot.r[2] = cameraAxisZ;
//	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
//
//	//�]�u�ɂ��t�s��i�t��]�j���v�Z
//	cameraInfo.matView = XMMatrixTranspose(matCameraRot);
//
//	//���_���W��-1�����������W
//	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
//	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
//	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
//	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
//	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
//	//��̃x�N�g���ɂ܂Ƃ߂�
//	XMVECTOR translateion = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
//
//	//�r���[�s��ɕ��s�ړ�������ݒ�
//	cameraInfo.matView.r[3] = translateion;
//
//#pragma region �S�����r���{�[�h�s��̌v�Z
//	//�r���{�[�h�s��
//	billBoardMat.r[0] = cameraAxisX;
//	billBoardMat.r[1] = cameraAxisY;
//	billBoardMat.r[2] = cameraAxisZ;
//	billBoardMat.r[3] = XMVectorSet(0, 0, 0, 1);
//
//#pragma endregion
//#pragma region Y�����r���{�[�h�s��̌v�Z
//	//�J����X���AY���AZ��
//	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
//
//	//X���͋���
//	ybillCameraAxisX = cameraAxisX;
//	//Y���̓��[���h���W�n��Y��
//	ybillCameraAxisY = XMVector3Normalize(upVector);
//	//Z����X����Y���̊O�ςŋ��܂�
//	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);
//
//	//Y�����r���{�[�h�s��
//	billBoardMatY.r[0] = ybillCameraAxisX;
//	billBoardMatY.r[1] = ybillCameraAxisY;
//	billBoardMatY.r[2] = ybillCameraAxisZ;
//	billBoardMatY.r[3] = XMVectorSet(0, 0, 0, 1);
//#pragma endregion
//
//	ConstMapping();
//}

GameCamera::GameCamera()
	:Camera("GameCamera"), motion(this)
{
	dirty.AddInspector(&shakeOffset.x);
	dirty.AddInspector(&shakeOffset.y);
	dirty.AddInspector(&shakeOffset.z);

	zoomMotion.easingChangeType = In;
	zoomMotion.easingType = Exp;
	zoomMotion.totalTime = ZOOM_CAM_MOTION_TOTAL_TIME;
	zoomMotion.endEye.z = ZOOM_CAM_MOTION_Z;
	zoomMotion.endTarget.z = ZOOM_CAM_MOTION_Z + 9.0f;

	returnDefaultMotion.easingChangeType = Out;
	returnDefaultMotion.easingType = Exp;
	returnDefaultMotion.totalTime = RETURN_DEFAULT_CAM_MOTION_TOTAL_TIME;
	returnDefaultMotion.endEye = DEFAULT_CAM_POS;
	returnDefaultMotion.endTarget = DEFAULT_CAM_TARGET;

	zoomToEnemyMotion.easingChangeType = Out;
	zoomToEnemyMotion.easingType = Exp;
	zoomToEnemyMotion.totalTime = 60;
	zoomToEnemyMotion.returnFlg = false;

	explosionMotion.easingChangeType = Out;
	explosionMotion.easingType = Exp;
	explosionMotion.totalTime = 110;
	explosionMotion.endEye.z = DEFAULT_CAM_POS.z - 30;
	explosionMotion.returnFlg = false;
}

GameCamera::~GameCamera()
{
}

void GameCamera::Init()
{
	dirty.Register();

	motion.Init();

	this->cameraInfo.eye = DEFAULT_CAM_POS;
	target = DEFAULT_CAM_TARGET;
	up = { 0, 1, 0 };

	shakeTimer = 0;
	shakeOffset = { 0.0f,0.0f,0.0f };

	UpdateViewMat();
	ConstMapping();

	//zoom = false;
}

void GameCamera::Update()
{
	if (!motion.NowMotion())
	{
		//�ʏ펞
		//���C���[�ő吔�ɍ��킹�ăY�[���A�E�g
		cameraInfo.eye.z = MyFunc::Lerp(cameraInfo.eye.z,
			DEFAULT_CAM_POS.z - (PanelMgr::Instance()->GetMaxLayerNum() - 1) * CAMERA_ZOOM_OUT_OFFSET, 0.1f);
	}
	//���[�V���������蓖�Ă��Ă���
	else
	{
		motion.Update();
	}

#pragma region �J�����V�F�C�N

	cameraInfo.eye = MyFunc::SubXMFLOAT3(cameraInfo.eye, shakeOffset);
	target = MyFunc::SubXMFLOAT3(target, shakeOffset);

	if (shakeTimer == 0)
	{
		shakeOffset = { 0,0,0 };
	}
	else
	{
		float rate = (float)shakeTimer / (float)shakeSpan;
		shakeOffset.x = MyFunc::GetRand(shakeRandMin, shakeRandMax) * rate * MyFunc::GetRandPlusMinus();
		shakeOffset.y = MyFunc::GetRand(shakeRandMin, shakeRandMax) * rate * MyFunc::GetRandPlusMinus();
		//shakeOffset.z = MyFunc::GetRand(SHAKE_POWER_MIN, SHAKE_POWER_MAX) * rate * MyFunc::GetRandPlusMinus();
		shakeTimer--;
	}

	cameraInfo.eye = MyFunc::AddXMFLOAT3(cameraInfo.eye, shakeOffset);
	target = MyFunc::AddXMFLOAT3(target, shakeOffset);
#pragma endregion

	if (this->dirty.Dirty())
	{
		UpdateViewMat();
	}
}

void GameCamera::ZoomToCenter()
{
	//if (!zoom)
	//{
		motion.StartMotion(zoomMotion);
	//}
	//zoom = true;
}

void GameCamera::ReturnToDefault()
{
	//zoom = false;
	returnDefaultMotion.endEye.z = DEFAULT_CAM_POS.z - (PanelMgr::Instance()->GetMaxLayerNum() - 1) * CAMERA_ZOOM_OUT_OFFSET;
	motion.StartMotion(returnDefaultMotion);	//��ʒu�ɖ߂郂�[�V����
}

void GameCamera::ExplosionCam(const XMFLOAT3& DeadEnemyPos)
{
	zoomToEnemyMotion.endEye = DeadEnemyPos;
	zoomToEnemyMotion.endEye.z -= 15;
	zoomToEnemyMotion.endTarget = DeadEnemyPos;

	explosionMotion.endEye.z = cameraInfo.eye.z - 30;
	motion.StartMotion(zoomToEnemyMotion);
}

void GameCamera::ExplosionCam()
{
	motion.StartMotion(explosionMotion);
}

void GameCamera::CamShake()
{
	shakeTimer = SHAKE_SPAN;
	shakeSpan = SHAKE_SPAN;
	shakeRandMax = SHAKE_POWER_MAX;
	shakeRandMin = SHAKE_POWER_MIN;
}

//void GameCamera::SetMotion(ActEditorsForCam* ActiveEditor, bool ActRemove)
//{
//	autoActRemove = ActRemove;
//	//ActiveEditor->SetTarget(&cameraInfo.eye, &rotate);
//	actEditors = ActiveEditor;
//	actEditors->Init();
//	UpdateViewMat();
//
//	actEditors->AllOn();
//}
//
//bool GameCamera::GetMotionMatch(ActEditorsForCam* ActiveEditor)
//{
//	return actEditors == ActiveEditor;
//}