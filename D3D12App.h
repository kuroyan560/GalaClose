#pragma once
#include "Singleton.h"

#include <d3d12.h>
#include<dxgi1_6.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include<wrl.h>
#include<vector>

#include"DescHeap.h"

class D3D12App : public Singleton<D3D12App>
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	static float CLEAR_COLOR[4];

	D3D12App();
	friend Singleton<D3D12App>;
	virtual ~D3D12App();

	ComPtr<ID3D12Device>dev = nullptr;
	ComPtr<IDXGIFactory6>dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4>swapchain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	std::vector<ComPtr<ID3D12Resource>>backBuffers;
	UINT bbIndex;
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	DescHeap* rtvHeap = nullptr;

public:
	void PreDraw();	//リソースバリアの変更と画面クリア
	void PostDraw();	//コマンドの実行と描画、裏表バッファをフリップ

	ID3D12Device* GetDev() { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

	void SetFrameRenderTarget();
};

