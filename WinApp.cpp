#include "WinApp.h"
#include"MyFunc.h"
#include"src/users/Constants.h"

const wchar_t* WinApp::WIN_NAME = L"�M�����N���[�Y";

int WinApp::WIN_WIDTH = 0;
int WinApp::WIN_HEIGHT = 0;

XMMATRIX WinApp::PARALLEL_MAT_PROJECTION = XMMatrixIdentity();

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

//�V�X�e�����b�Z�[�W���������邽�߂̊֐��A�E�B���h�E�v���V�[�W�������
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: //�E�B���h�E���j�����ꂽ
		PostQuitMessage(0);	//OS�ɑ΂��ăA�v���̏I����`����
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);	//�W���̏������s��
}

WinApp::WinApp()
{
	fullScreen = FULL_SCREEN;
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;	//�E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = L"�M�����N���[�Y";	//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);	//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);


	// Make the window borderless so that the client area can fill the screen.
	//AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);	//�����ŃT�C�Y�␳

	if (fullScreen)
	{
		// Get the settings of the primary display
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		//�E�B���h�E�T�C�Y{ X���W�@Y���W�@�����@�c�� }
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

		//�E�B���h�E�T�C�Y{ X���W�@Y���W�@�����@�c�� }
		windowRect = { 0,0,WIN_WIDTH,WIN_HEIGHT };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);	//�����ŃT�C�Y�␳
	}

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(w.lpszClassName,//�N���X��
		WIN_NAME,						//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,				//�W���I�ȃE�B���h�E�X�^�C��
		0/*CW_USEDEFAULT*/,						//�\��X���W�iOS�ɔC����j
		0/*CW_USEDEFAULT*/,						//�\��Y���W�iOS�ɔC����j
		WIN_WIDTH,				//�E�B���h�E����
		WIN_HEIGHT,				//�E�B���h�E�c��
		nullptr,							//�e�E�B���h�E�n���h��
		nullptr,							//���j���[�n���h��
		w.hInstance,						//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);							//�I�v�V����

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

		//�E�B���h�E�\��
		ShowWindow(hwnd, SW_MAXIMIZE);
	}
	else
	{
		//�E�B���h�E�\��
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
	//�E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
}

int WinApp::MessageLoop()
{
	//���b�Z�[�W������H
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�I�����b�Z�[�W�������烋�[�v�𔲂���
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
	MessageBox(hwnd, Message, TEXT("�G���[����"), MB_OK);
	DebugBreak();
}