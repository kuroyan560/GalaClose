#include "UsersInput.h"
#include"WinApp.h"
#include <cassert>
#include"CameraMgr.h"
#include"WinApp.h"
#include<DirectXMath.h>

UsersInput::UsersInput()
{
	HRESULT result;
	//DirectInputオブジェクトの生成
	if (FAILED(result = DirectInput8Create(
		WinApp::Instance()->GetWNDCLASSEX().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr)))assert(0);

	//キーボードデバイスの生成
	if (FAILED(result = dinput->CreateDevice(GUID_SysKeyboard, &keyDev, NULL)))assert(0);
	if (FAILED(result = keyDev->SetDataFormat(&c_dfDIKeyboard)))assert(0);	//標準形式
	if (FAILED(result = keyDev->SetCooperativeLevel(		//排他制御レベルのセット
		WinApp::Instance()->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/)))assert(0);

	//マウスデバイスの生成
	if (FAILED(result = dinput->CreateDevice(GUID_SysMouse, &mouseDev, NULL)))assert(0);
	if (FAILED(result = mouseDev->SetDataFormat(&c_dfDIMouse2)))assert(0);
	if (FAILED(result = mouseDev->SetCooperativeLevel(
		WinApp::Instance()->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/)))assert(0);

	/*
		使っているフラグについて
		DISCL_FOREGROUND	画面が手前にある場合のみ入力を受け付ける
		DISCL_NONEXCLUSIVE	デバイスをこのアプリだけで専有しない
		DISCL_NOWINKEY		Windowsキーを無効にする
	*/
}

void UsersInput::Update()
{
	//キーボード
	memcpy(oldkeys, keys, KEY_NUM);
	//キーボード情報の取得開始
	HRESULT result = keyDev->Acquire();	//本当は一回でいいが、アプリが裏面から全面に戻る度呼び出す必要があるため、毎フレーム呼び出す。
	//全キーの入力状態を取得する
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

	//マウス
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