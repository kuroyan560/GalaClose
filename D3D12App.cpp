#include "D3D12App.h"

#include<string>
#include"WinApp.h"
#include"d3dx12.h"
#include"MyFunc.h"

//float D3D12App::CLEAR_COLOR[] = { 0.0,0.0,0.0,0.0 };
float D3D12App::CLEAR_COLOR[] = { 0.1f, 0.25f, 0.5f, 1.0f };	//���ۂ��F
//float D3D12App::CLEAR_COLOR[] = { 0.098f,0.082f,0.282f,1.0f };

D3D12App::D3D12App()
{
	HRESULT result;

	//�O���t�B�b�N�X�{�[�h�̃A�_�v�^���
	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	//�A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter>>adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);		//���I�z��ɒǉ�����
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);	//�A�_�v�^�[�̏����擾
		std::wstring strDesc = adesc.Description;	//�A�_�v�^�[��
		//Microsoft Basic Render Driver,Intel UHD Graphics �����
		if (strDesc.find(L"Microsoft") == std::wstring::npos
			&& strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];	//�̗p
			break;
		}
	}

	//�f�o�C�X�̐����iDirect3D12�̊�{�I�u�W�F�N�g�j
	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

	//�R�}���h�A���P�[�^�𐶐�
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	//�R�}���h���X�g�𐶐��iGPU�ɁA�܂Ƃ߂Ė��߂𑗂邽�߂̃R�}���h���X�g�𐶐�����j
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//�R�}���h�L���[�̐����i�R�}���h���X�g��GPU�ɏ��Ɏ��s�����Ă����ׂ̎d�g�݂𐶐�����j
	//�W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	WinApp::Instance();
	//�X���b�v�`�F�[���̐���
	//�e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = WinApp::WIN_WIDTH;
	swapchainDesc.Height = WinApp::WIN_HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�F���̏���
	swapchainDesc.SampleDesc.Count = 1;	//�}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	//�o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2;	//�o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//�t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1>swapchain1;
	//�X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		WinApp::Instance()->GetHWND(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);
	if (FAILED(result))
	{
		DebugBreak();
	}
	//��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapchain1.As(&swapchain);
	//�f�B�X�N���v�^�q�[�v�ƃ����_�[�^�[�Q�b�g�r���[�̐���
	rtvHeap = new DescHeap(RTV, dev.Get());
	//���\�̂Q���ɂ���
	backBuffers.resize(2);
	for (int i = 0; i < 2; i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		//�f�B�X�N���v�^�q�[�v�̃n���h�����擾
		//�����\�ŃA�h���X�������
		//�����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			rtvHeap->GetCpuHandle(i));
		rtvHeap->AddDescNum();
	}

	//�t�F���X�iCPU��GPU�œ������Ƃ邽�߂̎d�g�݁j
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	//�[�x�o�b�t�@�̍쐬
	//CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	DXGI_FORMAT_D32_FLOAT,	//�[�x�l�t�H�[�}�b�g
	//	WinApp::WIN_WIDTH,	//�����_�[�^�[�Q�b�g�ɍ��킹��
	//	WinApp::WIN_HEIGHT,
	//	1, 0,
	//	1, 0,
	//	D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);	//�f�v�X�X�e���V��
}

D3D12App::~D3D12App()
{
	MyFunc::SafetyDelete(rtvHeap);
}

void D3D12App::PreDraw()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾�i�Q�Ȃ̂łO�Ԃ��P�ԁj
	bbIndex = swapchain->GetCurrentBackBufferIndex();

	//�\����Ԃ���`���ԂɕύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Activity(
		backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	//RTV��DSV�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeap->GetCpuHandle(bbIndex);

	//�����_�[�^�[�Q�b�g�ݒ�
	cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

	//��ʃN���A�R�}���h�i���\�[�X�o���A�̊Ԃɑ}���j
	cmdList->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);

	//�r���[�|�[�g�ݒ�
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::WIN_WIDTH, WinApp::WIN_HEIGHT));

	//�V�U�[��`�ݒ�
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::WIN_WIDTH, WinApp::WIN_HEIGHT));
}

void D3D12App::PostDraw()
{
	//�`���Ԃ���\����ԂɕύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Activity(
		backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	//���߂̃N���[�Y
	cmdList->Close();

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };	//�R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	//�R�}���h���X�g�̎��s������҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();	//�L���[���N���A
	cmdList->Reset(cmdAllocator.Get(), nullptr);		//�ĂуR�}���h���X�g�𒙂߂鏀��

	//�o�b�t�@���t���b�v�i���\�̓���ւ��j
	swapchain->Present(1, 0);
}

void D3D12App::SetFrameRenderTarget()
{
	//�t���[���o�b�t�@���Z�b�g
	//RTV��DSV�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeap->GetCpuHandle(bbIndex);

	//�����_�[�^�[�Q�b�g�ݒ�
	cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
}
