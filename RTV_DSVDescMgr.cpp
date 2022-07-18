#include "RTV_DSVDescMgr.h"
#include"D3D12App.h"

RTV_DSVDescMgr::RTV_DSVDescMgr()
	:rtvHeap(RTV, D3D12App::Instance()->GetDev()), dsvHeap(DSV, D3D12App::Instance()->GetDev())
{
}

int RTV_DSVDescMgr::CreateRTV(ID3D12Resource* RenderTarget)
{
	int handle;
	D3D12App::Instance()->GetDev()->CreateRenderTargetView(
		RenderTarget, nullptr, rtvHeap.GetCpuHandle());
	handle = rtvHeap.GetDescNum();
	rtvHeap.AddDescNum();
	return handle;
}

int RTV_DSVDescMgr::CreateDSV(ID3D12Resource* DepthStencil)
{
	int handle;
	D3D12App::Instance()->GetDev()->CreateDepthStencilView(
		DepthStencil, nullptr, dsvHeap.GetCpuHandle());
	handle = dsvHeap.GetDescNum();
	dsvHeap.AddDescNum();
	return handle;
}
