#pragma once
#include"DescHeap.h"
#include"Singleton.h"
class RTV_DSVDescMgr : public Singleton<RTV_DSVDescMgr>
{
	friend class Singleton<RTV_DSVDescMgr>;

	//共有RTVディスクリプタヒープ
	DescHeap rtvHeap;
	//共有DSVディスクリプタヒープ
	DescHeap dsvHeap;

	RTV_DSVDescMgr();
public:
	int CreateRTV(ID3D12Resource* RenderTarget);
	int CreateDSV(ID3D12Resource* DepthStencil);

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandleRTV(int Handle)
	{
		return rtvHeap.GetCpuHandle(Handle);
	}
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandleRTV(int Handle)
	{
		return rtvHeap.GetGpuHandle(Handle);
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandleDSV(int Handle)
	{
		return dsvHeap.GetCpuHandle(Handle);
	}
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandleDSV(int Handle)
	{
		return dsvHeap.GetGpuHandle(Handle);
	}
};

