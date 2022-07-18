#include "UsersInput.h"
#include"WinApp.h"
#include <cassert>
#include"CameraMgr.h"
#include"WinApp.h"
#include<DirectXMath.h>

UsersInput::UsersInput()
{
	HRESULT result;
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(result = DirectInput8Create(
		WinApp::Instance()->GetWNDCLASSEX().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr)))assert(0);

	//�L�[�{�[�h�f�o�C�X�̐���
	if (FAILED(result = dinput->CreateDevice(GUID_SysKeyboard, &keyDev, NULL)))assert(0);
	if (FAILED(result = keyDev->SetDataFormat(&c_dfDIKeyboard)))assert(0);	//�W���`��
	if (FAILED(result = keyDev->SetCooperativeLevel(		//�r�����䃌�x���̃Z�b�g
		WinApp::Instance()->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/)))assert(0);

	//�}�E�X�f�o�C�X�̐���
	if (FAILED(result = dinput->CreateDevice(GUID_SysMouse, &mouseDev, NULL)))assert(0);
	if (FAILED(result = mouseDev->SetDataFormat(&c_dfDIMouse2)))assert(0);
	if (FAILED(result = mouseDev->SetCooperativeLevel(
		WinApp::Instance()->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/)))assert(0);

	/*
		�g���Ă���t���O�ɂ���
		DISCL_FOREGROUND	��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
		DISCL_NONEXCLUSIVE	�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
		DISCL_NOWINKEY		Windows�L�[�𖳌��ɂ���
	*/
}

void UsersInput::Update()
{
	//�L�[�{�[�h
	memcpy(oldkeys, keys, KEY_NUM);
	//�L�[�{�[�h���̎擾�J�n
	HRESULT result = keyDev->Acquire();	//�{���͈��ł������A�A�v�������ʂ���S�ʂɖ߂�x�Ăяo���K�v�����邽�߁A���t���[���Ăяo���B
	//�S�L�[�̓��͏�Ԃ��擾����
	result = keyDev->GetDeviceState(sizeof(keys), keys);

	onAnyKey = false;
	for (int i = 0; i < KEY_NUM; ++i)
	{
		if (!oldkeys[i] && keys[i])
		{
			onAnyKey = true;
			break;
		}
	}

	//�}�E�X
	result = mouseDev->Acquire();
	oldMouseState = mouseState;
	result = mouseDev->GetDeviceState(sizeof(mouseState), &mouseState);


	POINT p;
	GetCursorPos(&p);
	ScreenToClient(FindWindowA("DirectXGame", nullptr), &p);
	mousePos.x = p.x;
	mousePos.x = MyFunc::Clamp(0, WinApp::WIN_WIDTH, mousePos.x);
	mousePos.y = p.y;
	mousePos.y = MyFunc::Clamp(0, WinApp::WIN_HEIGHT, mousePos.y);
}

bool UsersInput::OnTrigger(int KeyCode)const
{
	return (!oldkeys[KeyCode] && keys[KeyCode]);
}

bool UsersInput::Input(int KeyCode)const
{
	return keys[KeyCode];
}

bool UsersInput::OffTrigger(int KeyCode) const
{
	return (oldkeys[KeyCode] && !keys[KeyCode]);
}

bool UsersInput::OffPut(int KeyCode) const
{
	return !keys[KeyCode];
}

bool UsersInput::OnTrigger(MOUSE Button) const
{
	return (!oldMouseState.rgbButtons[Button] && mouseState.rgbButtons[Button]);
}

bool UsersInput::Input(MOUSE Button) const
{
	return mouseState.rgbButtons[Button];
}

bool UsersInput::OffTrigger(MOUSE Button) const
{
	return (oldMouseState.rgbButtons[Button] && !mouseState.rgbButtons[Button]);
}

bool UsersInput::OffPut(MOUSE Button) const
{
	return !mouseState.rgbButtons[Button];
}

UsersInput::MouseMove UsersInput::GetMouseMove()
{
	MouseMove tmp;
	tmp.IX = mouseState.lX;
	tmp.IY = mouseState.lY;
	tmp.IZ = mouseState.lZ;
	return tmp;
}

const Ray& UsersInput::GetMouseRay()
{
	XMFLOAT3 start = MyFunc::ConvertScreenToWorld(mousePos, 0.0f);
	mouseRay.start = { start.x,start.y,start.z,1 };

	XMFLOAT3 to = MyFunc::ConvertScreenToWorld(mousePos, 1.0f);
	XMFLOAT3 vec(to.x - start.x, to.y - start.y, to.z - start.z);
	mouseRay.dir = XMLoadFloat3(&vec);
	mouseRay.dir = XMVector3Normalize(mouseRay.dir);

	return mouseRay;
}