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
//	////注視点の向き変え
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
//	//視点座標
//	XMFLOAT3 tmp = MyFunc::AddXMFLOAT3(cameraInfo.eye, shakeOffset);
//	XMVECTOR eyePosition = XMLoadFloat3(&tmp);
//	//注視点座標
//	//tmp = MyFunc::AddXMFLOAT3(target, shakeOffset);
//	//XMVECTOR targetPosition = XMLoadFloat3(&tmp);
//	//（仮の）上方向
//	XMVECTOR upVector = XMLoadFloat3(&up);
//
//	//カメラZ軸（視線方向）
//	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
//
//	//０ベクトルだと向きが定まらないので除外
//	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
//	assert(!XMVector3IsInfinite(cameraAxisZ));
//	assert(!XMVector3Equal(upVector, XMVectorZero()));
//	assert(!XMVector3IsInfinite(upVector));
//
//	//ベクトルを正規化
//	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
//
//	//カメラのX軸（右方向）
//	XMVECTOR cameraAxisX;
//	//X軸は上方向→Z軸の外積で求まる
//	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
//	//ベクトルを正規化
//	cameraAxisX = XMVector3Normalize(cameraAxisX);
//
//	//カメラのY軸（上方向）
//	XMVECTOR cameraAxisY;
//	//Y軸はZ軸→X軸の外積で求まる
//	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
//
//	//カメラ回転行列
//	XMMATRIX matCameraRot;
//	//カメラ座標系→ワールド座標系の変換行列
//	matCameraRot.r[0] = cameraAxisX;
//	matCameraRot.r[1] = cameraAxisY;
//	matCameraRot.r[2] = cameraAxisZ;
//	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
//
//	//転置により逆行列（逆回転）を計算
//	cameraInfo.matView = XMMatrixTranspose(matCameraRot);
//
//	//視点座標に-1をかけた座標
//	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
//	//カメラの位置からワールド原点へのベクトル（カメラ座標系）
//	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
//	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
//	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
//	//一つのベクトルにまとめる
//	XMVECTOR translateion = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
//
//	//ビュー行列に平行移動成分を設定
//	cameraInfo.matView.r[3] = translateion;
//
//#pragma region 全方向ビルボード行列の計算
//	//ビルボード行列
//	billBoardMat.r[0] = cameraAxisX;
//	billBoardMat.r[1] = cameraAxisY;
//	billBoardMat.r[2] = cameraAxisZ;
//	billBoardMat.r[3] = XMVectorSet(0, 0, 0, 1);
//
//#pragma endregion
//#pragma region Y軸回りビルボード行列の計算
//	//カメラX軸、Y軸、Z軸
//	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
//
//	//X軸は共通
//	ybillCameraAxisX = cameraAxisX;
//	//Y軸はワールド座標系のY軸
//	ybillCameraAxisY = XMVector3Normalize(upVector);
//	//Z軸はX軸→Y軸の外積で求まる
//	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);
//
//	//Y軸回りビルボード行列
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
		//通常時
		//レイヤー最大数に合わせてズームアウト
		cameraInfo.eye.z = MyFunc::Lerp(cameraInfo.eye.z,
			DEFAULT_CAM_POS.z - (PanelMgr::Instance()->GetMaxLayerNum() - 1) * CAMERA_ZOOM_OUT_OFFSET, 0.1f);
	}
	//モーションが割り当てられている
	else
	{
		motion.Update();
	}

#pragma region カメラシェイク

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
	motion.StartMotion(returnDefaultMotion);	//定位置に戻るモーション
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