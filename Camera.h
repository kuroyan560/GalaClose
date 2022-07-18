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
		XMMATRIX matView; // �r���[�v���W�F�N�V�����s��
		XMMATRIX matProjection;
		XMFLOAT3 eye; // �J�������W�i���[���h���W�j
	};
	ComPtr<ID3D12Resource> constBuff;

	CameraConstBuff cameraInfo;

	XMFLOAT3 target;	//�����_�i���[���h���ŃJ���������Ă���_�j
	// ������x�N�g��
	XMFLOAT3 up;

	//�r���{�[�h�s��
	//�S����
	XMMATRIX billBoardMat;
	//Y��
	XMMATRIX billBoardMatY;

	DirtyFlag dirty;

	Camera(string Name);

	//�ړ�
	void MoveVector(const XMFLOAT3& Move);
	void MoveVector(const XMVECTOR& Move);

	virtual void ConstMapping();
	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual void UpdateViewMat();
	void SetConstBuff(int CamRootParam);

	bool Dirty() { return dirty.Dirty(); }

public:
	const string name; //�J�����̖��O

	//�Z�b�^
	void SetPos(XMFLOAT3 Pos) { cameraInfo.eye = Pos; }
	void SetUp(XMFLOAT3 Up) { up = Up; }
	void SetTarget(XMFLOAT3 Target) { target = Target; }

	//�Q�b�^
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