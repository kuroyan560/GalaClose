#include "Camera.h"
#include"WinApp.h"
#include"D3D12App.h"

void Camera::UpdateViewMat()
{
	//視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&cameraInfo.eye);
	//注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	//（仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	//カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//０ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	//Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//転置により逆行列（逆回転）を計算
	cameraInfo.matView = XMMatrixTranspose(matCameraRot);

	//視点座標に-1をかけた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	//一つのベクトルにまとめる
	XMVECTOR translateion = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//ビュー行列に平行移動成分を設定
	cameraInfo.matView.r[3] = translateion;

#pragma region 全方向ビルボード行列の計算
	//ビルボード行列
	billBoardMat.r[0] = cameraAxisX;
	billBoardMat.r[1] = cameraAxisY;
	billBoardMat.r[2] = cameraAxisZ;
	billBoardMat.r[3] = XMVectorSet(0, 0, 0, 1);

#pragma endregion
#pragma region Y軸回りビルボード行列の計算
	//カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	//Y軸回りビルボード行列
	billBoardMatY.r[0] = ybillCameraAxisX;
	billBoardMatY.r[1] = ybillCameraAxisY;
	billBoardMatY.r[2] = ybillCameraAxisZ;
	billBoardMatY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion

	ConstMapping();
}

void Camera::SetConstBuff(int CamRootParam)
{
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(CamRootParam, constBuff->GetGPUVirtualAddress());
}

void Camera::ConstMapping()
{
	HRESULT result;
	// 定数バッファへデータ転送
	CameraConstBuff* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		memcpy(constMap, &cameraInfo, sizeof(CameraConstBuff));
		constBuff->Unmap(0, nullptr);
	}
}

void Camera::MoveVector(const XMFLOAT3& Move)
{
	cameraInfo.eye.x += Move.x;
	cameraInfo.eye.y += Move.y;
	cameraInfo.eye.z += Move.z;

	target.x += Move.x;
	target.y += Move.y;
	target.z += Move.z;
}

void Camera::MoveVector(const XMVECTOR& Move)
{
	cameraInfo.eye.x += Move.m128_f32[0];
	cameraInfo.eye.y += Move.m128_f32[1];
	cameraInfo.eye.z += Move.m128_f32[2];

	target.x += Move.m128_f32[0];
	target.y += Move.m128_f32[1];
	target.z += Move.m128_f32[2];
}

Camera::Camera(string Name) :name(Name)
{
	HRESULT result;
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CameraConstBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result))
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "カメラの定数バッファ生成に失敗しました");
	}

	cameraInfo.eye = { 0,0,-10 };
	target = { 0,0,0 };
	up = { 0, 1, 0 };

	cameraInfo.matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),								//画角
		(float)WinApp::WIN_WIDTH / WinApp::WIN_HEIGHT,	//アスペクト比
		0.1f, 3000.0f);		//前端、奥端

	dirty.AddInspector(&cameraInfo.eye.x);
	dirty.AddInspector(&cameraInfo.eye.y);
	dirty.AddInspector(&cameraInfo.eye.z);
	dirty.AddInspector(&target.x);
	dirty.AddInspector(&target.y);
	dirty.AddInspector(&target.z);
}

NonMoveCam::NonMoveCam()
	:Camera("NoMoveCamera")
{
	cameraInfo.eye.z = -50;
	UpdateViewMat();
}
