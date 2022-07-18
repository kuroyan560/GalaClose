#pragma once
#include<Windows.h>
#include"Singleton.h"
#include<DirectXMath.h>
#include<string>
#include"MyFunc.h"

class WinApp : public Singleton<WinApp>
{
private:
	WinApp();
	friend Singleton<WinApp>;

	static const wchar_t* WIN_NAME;

public:
	static XMMATRIX PARALLEL_MAT_PROJECTION;	//�ˉe�s��
	static int WIN_WIDTH;
	static int WIN_HEIGHT;
	static DirectX::XMFLOAT2 GetWinCenter();
	static MyDef::int2 GetWinSize();

private:
	WNDCLASSEX w{};	//�E�B���h�E�N���X�̐ݒ�
	MSG msg{};
	HWND hwnd;

	bool fullScreen = false;
	DirectX::XMFLOAT2 windowRate = { 1.0f,1.0f };
	RECT windowRect;

public:
	~WinApp();

	int MessageLoop();
	void CreateMessageWindow(std::string WindowName, std::string Comment);
	//�͂� or ������ �̃��b�Z�[�W�{�b�N�X
	void CreateMessageWindow(std::string WindowName, std::string Comment, bool* Flag);
	void DebugError(const wchar_t* Message);

	//�E�B���h�E���[�h����t���X�N���[����������Ƃ��̔䗦�̈Ⴂ
	const DirectX::XMFLOAT2 GetWinDifferRate() { return windowRate; }

	const HWND& GetHWND() { return hwnd; }
	const WNDCLASSEX& GetWNDCLASSEX() { return w; }
};