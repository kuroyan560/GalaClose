#include "D3D12App.h"

#include<string>
#include"WinApp.h"
#include"d3dx12.h"
#include"MyFunc.h"

//float D3D12App::CLEAR_COLOR[] = { 0.0,0.0,0.0,0.0 };
float D3D12App::CLEAR_COLOR[] = { 0.1f, 0.25f, 0.5f, 1.0f };	//青っぽい色
//float D3D12App::CLEAR_COLOR[] = { 0.098f,0.082f,0.282f,1.0f };

D3D12App::D3D12App()
{
	HRESULT result;

	//グラフィックスボードのアダプタを列挙
	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	//アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter>>adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);		//動的配列に追加する
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);	//アダプターの情報を取得
		std::wstring strDesc = adesc.Description;	//アダプター名
		//Microsoft Basic Render Driver,Intel UHD Graphics を回避
		if (strDesc.find(L"Microsoft") == std::wstring::npos
			&& strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];	//採用
			break;
		}
	}

	//デバイスの生成（Direct3D12の基本オブジェクト）
	//対応レベルの配列
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
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	//コマンドアロケータを生成
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	//コマンドリストを生成（GPUに、まとめて命令を送るためのコマンドリストを生成する）
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//コマンドキューの生成（コマンドリストをGPUに順に実行させていく為の仕組みを生成する）
	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	WinApp::Instance();
	//スワップチェーンの生成
	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = WinApp::WIN_WIDTH;
	swapchainDesc.Height = WinApp::WIN_HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//色情報の書式
	swapchainDesc.SampleDesc.Count = 1;	//マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	//バックバッファ用
	swapchainDesc.BufferCount = 2;	//バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	//フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//IDXGISwapChain1のComPtrを用意
	ComPtr<IDXGISwapChain1>swapchain1;
	//スワップチェーンの生成
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
	//生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapchain1.As(&swapchain);
	//ディスクリプタヒープとレンダーターゲットビューの生成
	rtvHeap = new DescHeap(RTV, dev.Get());
	//裏表の２つ分について
	backBuffers.resize(2);
	for (int i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		//ディスクリプタヒープのハンドルを取得
		//裏か表でアドレスがずれる
		//レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			rtvHeap->GetCpuHandle(i));
		rtvHeap->AddDescNum();
	}

	//フェンス（CPUとGPUで同期をとるための仕組み）
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	//深度バッファの作成
	//CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	DXGI_FORMAT_D32_FLOAT,	//深度値フォーマット
	//	WinApp::WIN_WIDTH,	//レンダーターゲットに合わせる
	//	WinApp::WIN_HEIGHT,
	//	1, 0,
	//	1, 0,
	//	D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);	//デプスステンシル
}

D3D12App::~D3D12App()
{
	MyFunc::SafetyDelete(rtvHeap);
}

void D3D12App::PreDraw()
{
	//バックバッファの番号を取得（２つなので０番か１番）
	bbIndex = swapchain->GetCurrentBackBufferIndex();

	//表示状態から描画状態に変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Activity(
		backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	//RTVとDSVのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeap->GetCpuHandle(bbIndex);

	//レンダーターゲット設定
	cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

	//画面クリアコマンド（リソースバリアの間に挿入）
	cmdList->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);

	//ビューポート設定
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::WIN_WIDTH, WinApp::WIN_HEIGHT));

	//シザー矩形設定
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::WIN_WIDTH, WinApp::WIN_HEIGHT));
}

void D3D12App::PostDraw()
{
	//描画状態から表示状態に変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Activity(
		backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	//命令のクローズ
	cmdList->Close();

	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };	//コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	//コマンドリストの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();	//キューをクリア
	cmdList->Reset(cmdAllocator.Get(), nullptr);		//再びコマンドリストを貯める準備

	//バッファをフリップ（裏表の入れ替え）
	swapchain->Present(1, 0);
}

void D3D12App::SetFrameRenderTarget()
{
	//フレームバッファをセット
	//RTVとDSVのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeap->GetCpuHandle(bbIndex);

	//レンダーターゲット設定
	cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
}
