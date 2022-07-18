#pragma once
#include<wrl.h>
#include<d3dx12.h>
#include"WinApp.h"

class RenderTarget
{
public:
	//レンダーターゲットをセット
	static void SetRenderTarget(RenderTarget** RenderTargetArray, int Num);

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	//レンダーターゲットバッファ
	ComPtr<ID3D12Resource>renderTarget = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle;
	D3D12_RESOURCE_STATES rtvCurrentState;//リソースバリアの状態
	//レンダーターゲットテクスチャビューのハンドル(SRV)
	int srvHandle = -1;


	float rtvClearValue[4] = { 0.0f,0.0f,0.0f,1.0f };

	void CreateRenderTarget(DXGI_FORMAT ColorFormat, float* ClearValue, int Width, int Height, int MipLevel, int ArraySize);

public:
	RenderTarget(DXGI_FORMAT ColorFormat, float* ClearValue = nullptr,
		int Width = WinApp::WIN_WIDTH, int Height = WinApp::WIN_HEIGHT, int MipLevel = 1, int ArraySize = 1);

	//リソースバリアの変更
	void ChangeResourceBarrier(D3D12_RESOURCE_STATES State);

	//レンダーターゲットのクリア
	void Clear();

	ID3D12Resource* GetRenderTarget() { return renderTarget.Get(); }

	CD3DX12_CPU_DESCRIPTOR_HANDLE* GetRTVCpuHandle() { return &rtvCpuHandle; }

	//レンダーターゲットテクスチャとして使用
	int GetSRVHandle();
};

