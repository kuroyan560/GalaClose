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
	static XMMATRIX PARALLEL_MAT_PROJECTION;	//射影行列
	static int WIN_WIDTH;
	static int WIN_HEIGHT;
	static DirectX::XMFLOAT2 GetWinCenter();
	static MyDef::int2 GetWinSize();

private:
	WNDCLASSEX w{};	//ウィンドウクラスの設定
	MSG msg{};
	HWND hwnd;

	bool fullScreen = false;
	DirectX::XMFLOAT2 windowRate = { 1.0f,1.0f };
	RECT windowRect;

public:
	~WinApp();

	int MessageLoop();
	void CreateMessageWindow(std::string WindowName, std::string Comment);
	//はい or いいえ のメッセージボックス
	void CreateMessageWindow(std::string WindowName, std::string Comment, bool* Flag);
	void DebugError(const wchar_t* Message);

	//ウィンドウモードからフルスクリーンを作ったときの比率の違い
	const DirectX::XMFLOAT2 GetWinDifferRate() { return windowRate; }

	const HWND& GetHWND() { return hwnd; }
	const WNDCLASSEX& GetWNDCLASSEX() { return w; }
};