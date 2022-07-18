#include "Camera.h"
#include"WinApp.h"
#include"D3D12App.h"

void Camera::UpdateViewMat()
{
	//���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&cameraInfo.eye);
	//�����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	//�i���́j�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	//�J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//�O�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//�J������X���i�E�����j
	XMVECTOR cameraAxisX;
	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������Y���i������j
	XMVECTOR cameraAxisY;
	//Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]�s��
	XMMATRIX matCameraRot;
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//�]�u�ɂ��t�s��i�t��]�j���v�Z
	cameraInfo.matView = XMMatrixTranspose(matCameraRot);

	//���_���W��-1�����������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	//��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translateion = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//�r���[�s��ɕ��s�ړ�������ݒ�
	cameraInfo.matView.r[3] = translateion;

#pragma region �S�����r���{�[�h�s��̌v�Z
	//�r���{�[�h�s��
	billBoardMat.r[0] = cameraAxisX;
	billBoardMat.r[1] = cameraAxisY;
	billBoardMat.r[2] = cameraAxisZ;
	billBoardMat.r[3] = XMVectorSet(0, 0, 0, 1);

#pragma endregion
#pragma region Y�����r���{�[�h�s��̌v�Z
	//�J����X���AY���AZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	//Y�����r���{�[�h�s��
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
	// �萔�o�b�t�@�փf�[�^�]��
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
		WinApp::Instance()->CreateMessageWindow("ERROR", "�J�����̒萔�o�b�t�@�����Ɏ��s���܂���");
	}

	cameraInfo.eye = { 0,0,-10 };
	target = { 0,0,0 };
	up = { 0, 1, 0 };

	cameraInfo.matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),								//��p
		(float)WinApp::WIN_WIDTH / WinApp::WIN_HEIGHT,	//�A�X�y�N�g��
		0.1f, 3000.0f);		//�O�[�A���[

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
