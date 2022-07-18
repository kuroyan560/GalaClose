#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include"DirtyFlag.h"
#include<string>
using namespace std;

#include<wrl.h>

#include"MyFunc.h"

#include<d3d12.h>
using namespace DirectX;

class Camera
{
protected:
	friend void ImguiCommand_Master();
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	friend class CameraMgr;

	struct CameraConstBuff
	{
		XMMATRIX matView; // ビュープロジェクション行列
		XMMATRIX matProjection;
		XMFLOAT3 eye; // カメラ座標（ワールド座標）
	};
	ComPtr<ID3D12Resource> constBuff;

	CameraConstBuff cameraInfo;

	XMFLOAT3 target;	//注視点（ワールド内でカメラが見ている点）
	// 上方向ベクトル
	XMFLOAT3 up;

	//ビルボード行列
	//全方向
	XMMATRIX billBoardMat;
	//Y軸
	XMMATRIX billBoardMatY;

	DirtyFlag dirty;

	Camera(string Name);

	//移動
	void MoveVector(const XMFLOAT3& Move);
	void MoveVector(const XMVECTOR& Move);

	virtual void ConstMapping();
	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual void UpdateViewMat();
	void SetConstBuff(int CamRootParam);

	bool Dirty() { return dirty.Dirty(); }

public:
	const string name; //カメラの名前

	//セッタ
	void SetPos(XMFLOAT3 Pos) { cameraInfo.eye = Pos; }
	void SetUp(XMFLOAT3 Up) { up = Up; }
	void SetTarget(XMFLOAT3 Target) { target = Target; }

	//ゲッタ
	const XMMATRIX& MatView() { return cameraInfo.matView; }
	const XMMATRIX& MatProjection() { return cameraInfo.matProjection; }
	const XMMATRIX& MatBillBoard() { return billBoardMat; }
	const XMMATRIX& MatBillBoardY() { return billBoardMatY; }
	const XMFLOAT3& GetPos() { return cameraInfo.eye; }
	const XMFLOAT3& GetTarget() { return target; }
	const XMFLOAT3& GetFrontVec() { return MyFunc::SubXMFLOAT3(target, cameraInfo.eye); }
};

class NonMoveCam : public Camera
{
public:
	NonMoveCam();
	void SetConstBuff(int CamRootParam) { Camera::SetConstBuff(CamRootParam); }
	void Init()override { dirty.Register(); };
	void Update()override {};
};