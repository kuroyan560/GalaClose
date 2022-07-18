#include "RenderTarget.h"
#include"D3D12App.h"
#include"RTV_DSVDescMgr.h"
#include"SRVDescMgr.h"
#include"RenderTargetMgr.h"
void RenderTarget::SetRenderTarget(RenderTarget** RenderTargetArray, int Num)
{
	//メインレンダーターゲットのセット
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>handles;
	for (int i = 0; i < Num; ++i)
	{
		handles.emplace_back(RenderTargetArray[i]->rtvCpuHandle);
	}

	//メインのレンダーターゲットのデプスステンシルセット
	D3D12App::Instance()->GetCmdList()->OMSetRenderTargets(Num, &handles[0], FALSE,
		RenderTargetMgr::Instance()->GetDepthStencil());
}

void RenderTarget::CreateRenderTarget(DXGI_FORMAT ColorFormat, float* ClearValue, int Width, int Height, int MipLevel, int ArraySize)
{
	//レンダーターゲット生成
	CD3DX12_RESOURCE_DESC desc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		static_cast<UINT>(Width),
		static_cast<UINT>(Height),
		ArraySize,
		MipLevel,
		ColorFormat,
		1,
		0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//レンダーターゲットのクリア値
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = ColorFormat;
	if (ClearValue != nullptr) {
		clearValue.Color[0] = ClearValue[0];
		clearValue.Color[1] = ClearValue[1];
		clearValue.Color[2] = ClearValue[2];
		clearValue.Color[3] = ClearValue[3];

		rtvClearValue[0] = ClearValue[0];
		rtvClearValue[1] = ClearValue[1];
		rtvClearValue[2] = ClearValue[2];
		rtvClearValue[3] = ClearValue[3];
	}
	else {
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;
	}

	//リソース生成
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto hr = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		rtvCurrentState,
		&clearValue,
		IID_PPV_ARGS(&renderTarget)
	);

	if (FAILED(hr))
	{
		DebugBreak();
	}

	//RTVの作成
	int handle = RTV_DSVDescMgr::Instance()->CreateRTV(renderTarget.Get());
	rtvCpuHandle = RTV_DSVDescMgr::Instance()->GetCpuHandleRTV(handle);

	//SRVの作成
	srvHandle = SRVDescMgr::Instance()->CreateSRV(renderTarget.Get(), ColorFormat);
}

RenderTarget::RenderTarget(DXGI_FORMAT ColorFormat, float* ClearValue, int Width, int Height, int MipLevel, int ArraySize)
{
	rtvCurrentState = D3D12_RESOURCE_STATE_PRESENT;

	CreateRenderTarget(ColorFormat, ClearValue, Width, Height, MipLevel, ArraySize);
}

void RenderTarget::ChangeResourceBarrier(D3D12_RESOURCE_STATES State)
{
	if (State == rtvCurrentState)return;

	D3D12App::Instance()->GetCmdList()->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget.Get(),
			rtvCurrentState,
			State));
	rtvCurrentState = State;
}

void RenderTarget::Clear()
{
	ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	D3D12App::Instance()->GetCmdList()->ClearRenderTargetView(
		rtvCpuHandle,
		rtvClearValue,
		0,
		nullptr);
}

int RenderTarget::GetSRVHandle()
{
	//レンダーターゲットテクスチャとして使用するためにリソースバリア変更
	this->ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	return srvHandle;
}
