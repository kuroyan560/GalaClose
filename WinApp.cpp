#include "WinApp.h"
#include"MyFunc.h"
#include"src/users/Constants.h"

const wchar_t* WinApp::WIN_NAME = L"ギャラクローズ";

int WinApp::WIN_WIDTH = 0;
int WinApp::WIN_HEIGHT = 0;

XMMATRIX WinApp::PARALLEL_MAT_PROJECTION = XMMatrixIdentity();

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

//システムメッセージを処理するための関数、ウィンドウプロシージャを作る
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: //ウィンドウが破棄された
		PostQuitMessage(0);	//OSに対してアプリの終了を伝える
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);	//標準の処理を行う
}

WinApp::WinApp()
{
	fullScreen = FULL_SCREEN;
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;	//ウィンドウプロシージャを設定
	w.lpszClassName = L"ギャラクローズ";	//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);	//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);


	// Make the window borderless so that the client area can fill the screen.
	//AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);	//自動でサイズ補正

	if (fullScreen)
	{
		// Get the settings of the primary display
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		//ウィンドウサイズ{ X座標　Y座標　横幅　縦幅 }
		windowRect = {
			devMode.dmPosition.x,
			devMode.dmPosition.y,
			devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
			devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight)
		};

		WIN_WIDTH = windowRect.right - windowRect.left;
		WIN_HEIGHT = windowRect.bottom - windowRect.top;

		windowRate.x = (float)WIN_WIDTH / (float)WINDOW_WIDTH;
		windowRate.y = (float)WIN_HEIGHT / (float)WINDOW_HEIGHT;
	}
	else
	{
		WIN_WIDTH = WINDOW_WIDTH;
		WIN_HEIGHT = WINDOW_HEIGHT;

		//ウィンドウサイズ{ X座標　Y座標　横幅　縦幅 }
		windowRect = { 0,0,WIN_WIDTH,WIN_HEIGHT };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);	//自動でサイズ補正
	}

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,//クラス名
		WIN_NAME,						//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,				//標準的なウィンドウスタイル
		0/*CW_USEDEFAULT*/,						//表示X座標（OSに任せる）
		0/*CW_USEDEFAULT*/,						//表示Y座標（OSに任せる）
		WIN_WIDTH,				//ウィンドウ横幅
		WIN_HEIGHT,				//ウィンドウ縦幅
		nullptr,							//親ウィンドウハンドル
		nullptr,							//メニューハンドル
		w.hInstance,						//呼び出しアプリケーションハンドル
		nullptr);							//オプション

	if (fullScreen)
	{
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX /*| WS_SYSMENU*/ | WS_THICKFRAME));

		SetWindowPos(
			hwnd,
			HWND_TOPMOST,
			windowRect.left,
			windowRect.top,
			windowRect.right,
			windowRect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		//ウィンドウ表示
		ShowWindow(hwnd, SW_MAXIMIZE);
	}
	else
	{
		//ウィンドウ表示
		ShowWindow(hwnd, SW_SHOW);
	}

	PARALLEL_MAT_PROJECTION = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH, WIN_HEIGHT, 0.0f, 0.0f, 1.0f);
}
DirectX::XMFLOAT2 WinApp::GetWinCenter()
{
	DirectX::XMFLOAT2 center;
	center.x = (float)WIN_WIDTH / 2.0f;
	center.y = (float)WIN_HEIGHT / 2.0f;
	return center;
}
MyDef::int2 WinApp::GetWinSize()
{
	return MyDef::int2{ WIN_WIDTH,WIN_HEIGHT };
}
WinApp::~WinApp()
{
	//ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

int WinApp::MessageLoop()
{
	//メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	//キー入力メッセージの処理
		DispatchMessage(&msg);	//プロシージャにメッセージを送る
	}

	//終了メッセージが来たらループを抜ける
	if (msg.message == WM_QUIT)
	{
		return -1;
	}

	return 0;
}

void WinApp::CreateMessageWindow(std::string WindowName, std::string Comment)
{
	
	MessageBox(hwnd, MyFunc::GetWideStrFromStr(Comment).c_str(), MyFunc::GetWideStrFromStr(WindowName).c_str(), MB_OK);
}

void WinApp::CreateMessageWindow(std::string WindowName, std::string Comment, bool* Flag)
{
	int answer = MessageBox(hwnd, MyFunc::GetWideStrFromStr(Comment).c_str(), MyFunc::GetWideStrFromStr(WindowName).c_str(),
		MB_YESNO | MB_ICONQUESTION);
	*Flag = (answer == IDYES);
}

void WinApp::DebugError(const wchar_t* Message)
{
	MessageBox(hwnd, Message, TEXT("エラー発生"), MB_OK);
	DebugBreak();
}