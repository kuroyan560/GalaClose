#include "CameraMgr.h"
#include"UsersInput.h"
#include"MyFunc.h"
#include"Object3D.h"
#include"WinApp.h"
#include"GameCamera.h"

#pragma region デバッグカメラ
const XMFLOAT3 CameraMgr::DebugCamera::INIT_POS = { 0,10,-10 };
CameraMgr::DebugCamera::DebugCamera() : Camera("DebugCamera")
{
	target = { INIT_POS.x,INIT_POS.y,0 };
	cameraInfo.eye = { target.x,target.y,-distance };

	//画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)WinApp::WIN_WIDTH;
	scaleY = 1.0f / (float)WinApp::WIN_HEIGHT;

	UpdateViewMat();
}
void CameraMgr::DebugCamera::Update()
{
	bool moveDirty = false;
	float angleX = 0;
	float angleY = 0;

	//マウスの入力を取得
	UsersInput::MouseMove mouseMove = UsersInput::Instance()->GetMouseMove();

	//マウス左クリックでカメラ回転
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

	//マウス中クリックでカメラ平行移動
	if (UsersInput::Instance()->Input(UsersInput::MOUSE::CENTER))
	{
		float dx = mouseMove.IX / 100.0f;
		float dy = mouseMove.IY / 100.0f;

		XMVECTOR move = { -dx,+dy,0,0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		moveDirty = true;
	}

	//ホイール入力で距離を変更
	if (mouseMove.IZ != 0)
	{
		distance -= mouseMove.IZ / 100.0f;
		distance = max(distance, 1.0f);
		moveDirty = true;
	}

	if (moveDirty)
	{
		//追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// 注視点からずらした位置に視点座標を決定
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
