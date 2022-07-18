#pragma once
#include"Singleton.h"
#include"Camera.h"
#include<vector>

class CameraMgr : public Singleton<CameraMgr>
{
	class DebugCamera : public Camera
	{
		static const XMFLOAT3 INIT_POS;

		XMFLOAT2 rotate = { 0.0f,0.0f };
		// カメラ注視点までの距離
		float distance = 20;
		// スケーリング
		float scaleX = 1.0f;
		float scaleY = 1.0f;
		// 回転行列
		XMMATRIX matRot = DirectX::XMMatrixIdentity();

	public:
		DebugCamera();
		virtual void Init()override { dirty.Register(); };
		virtual void Update()override;
		const XMFLOAT2& GetRotate() { return rotate; }
	};

private:
	CameraMgr();
	friend Singleton<CameraMgr>;

	friend void ImguiCommand_Master();

	DebugCamera debugCam;
	std::vector<Camera*>cameras;
	static const int DEBUG_CAM_NUM = 0;
	int nowCameraNum = DEBUG_CAM_NUM;

public:
	~CameraMgr();

	void SetCam(int CameraNum = DEBUG_CAM_NUM);
	void Init();
	void SetCamConstBuff(int CamRootParam);
	void Update();
	bool Dirty();

	void RegisterCam(Camera* Cam) { cameras.push_back(Cam); }
	void ReleaseCam() { cameras.pop_back(); }

	const XMFLOAT3& GetPos() { return cameras[nowCameraNum]->GetPos(); }
	const XMFLOAT3& GetTarget() { return cameras[nowCameraNum]->target; }
	const XMMATRIX& GetView() { return cameras[nowCameraNum]->MatView(); }
	const XMMATRIX& GetProjection() { return cameras[nowCameraNum]->MatProjection(); }
	const XMMATRIX& GetBillboardMatrix() { return cameras[nowCameraNum]->MatBillBoard(); }
	const XMMATRIX& GetBillboardMatrixY() { return cameras[nowCameraNum]->MatBillBoardY(); }
	const XMFLOAT3& GetFrontVec() { return cameras[nowCameraNum]->GetFrontVec(); }

	const XMFLOAT2& GetDebugCamRotate() { return debugCam.GetRotate(); }
	const XMFLOAT3& GetDebugCamEye() { return debugCam.GetPos(); }
};
