#pragma region �K�{
#include<ctime>
#include<Windows.h>
#include<wrl.h>
using namespace Microsoft::WRL;
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")
#pragma endregion

#include"WinApp.h"
#include"SceneMgr.h"
#include"UsersInput.h"
#include"Sprite.h"
#include"OBJModel.h"
#include"Object2D.h"
#include"Particles.h"
#include"Model_HSM.h"

#include"BaseCollider.h"

#include"dxgidebug.h"
#include <dxgi1_3.h>

#include"Font.h"

#include"PrimitiveParticles.h"
#include"TimeShift.h"
#include"src/users/Layer.h"

#include"Fps.h"

#ifdef _DEBUG
int main()
#else
//Windows�A�v���ł̃G���g���[�|�C���g�imain�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	//�f�o�b�O���C���[
#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	HMODULE lib;
	lib = LoadLibrary(L"XAudio2_9.dll");

	Fps::Instance();

	std::srand(time(NULL));
	Sprite_Normal::CreatePipeline();
	OBJModel::CreatePipeline();
	Object2D::CreatePipeline();
	Particles::CreatePipeline();
	Model_HSM::CreatePipeline();
	Model_HSM_Front::CreatePipeline();
	PrimitivePaticles::CreatePipeline();

	Layer::CreatePipeline();

	//FontInitData initData;
	//initData.dir = "resource/font/";
	//initData.fontFileName = "font.fnt";
	//initData.loadChars = L"��������������ABCDEFabcdef�t��Bfd�H�~��";
	//Font test(initData);

	//���[�v
	while (1)
	{
		if (Fps::Instance()->Run())
		{
			if (TimeShift::Instance()->Update())
			{
				SceneMgr::Instance()->Update();
			}

			SceneMgr::Instance()->Draw();

			//���[�v�𔲂���
			if (WinApp::Instance()->MessageLoop() || SceneMgr::Instance()->End())
			{
				break;
			}
		}
	}

	FreeLibrary(lib);
}