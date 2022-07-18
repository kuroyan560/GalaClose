#pragma once
#include"Singleton.h"

#include <dinput.h>
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include<wrl.h>

#include"CollisionPrimitive.h"

class UsersInput : public Singleton<UsersInput>
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	UsersInput();
	friend Singleton<UsersInput>;

public:
	enum MOUSE { LEFT, RIGHT, CENTER };
	struct MouseMove
	{
		LONG IX;
		LONG IY;
		LONG IZ;
	};

private:
	ComPtr<IDirectInput8> dinput = nullptr;

	//キーボード
	ComPtr<IDirectInputDevice8> keyDev;
	static const int KEY_NUM = 256;
	BYTE keys[KEY_NUM] = {};
	BYTE oldkeys[KEY_NUM] = {};
	bool onAnyKey = false;

	//マウス
	ComPtr<IDirectInputDevice8>mouseDev;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};
	//マウスのゲーム空間内でのレイ
	DirectX::XMFLOAT2 mousePos;
	Ray mouseRay;

public:
	~UsersInput() {};

	void Update();

	//キーボード
	bool OnTrigger(int KeyCode)const;
	bool Input(int KeyCode)const;
	bool OffTrigger(int KeyCode)const;
	bool OffPut(int KeyCode)const;

	bool OnTriggerAnyKey()const { return onAnyKey; }

	//マウス
	bool OnTrigger(MOUSE Button)const;
	bool Input(MOUSE Button)const;
	bool OffTrigger(MOUSE Button)const;
	bool OffPut(MOUSE Button)const;
	const DirectX::XMFLOAT2& GetMousePos()const { return mousePos; }
	MouseMove GetMouseMove();
	const Ray& GetMouseRay();
};

