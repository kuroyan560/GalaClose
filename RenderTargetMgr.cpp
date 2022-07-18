#include "RenderTargetMgr.h"
#include"MyFunc.h"
#include"D3D12App.h"
#include"RTV_DSVDescMgr.h"

void RenderTargetMgr::CreateDepthStencil()
{
	CD3DX12_RESOURCE_DESC depthResDec = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,	//深度値フォーマット
		static_cast<UINT>(WinApp::WIN_WIDTH),	
		static_cast<UINT>(WinApp::WIN_HEIGHT),
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);	//デプスステンシル

	auto hr = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDec,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthStencil));

	if (FAILED(hr)) {
		//深度ステンシルバッファの作成に失敗。
		DebugBreak();
	}

	int handle = RTV_DSVDescMgr::Instance()->CreateDSV(depthStencil.Get());
	dsvCpuHandle = RTV_DSVDescMgr::Instance()->GetCpuHandleDSV(handle);
}

RenderTargetMgr::RenderTargetMgr()
{
	CreateDepthStencil();

	mainRenderTarget = new RenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, mainClearValue);//カラーマップ
	depthMapRenderTarget = new RenderTarget(DXGI_FORMAT_R32_FLOAT); //深度マップ
	clippingRenderTarget = new RenderTarget(DXGI_FORMAT_R32_FLOAT);//クリッピング用(0か1)

	recentRenderTarget = mainRenderTarget;

	PostEffectInitData data;
	data.name = "NonPostEffect";
	data.refferRenderTarget[0] = &recentRenderTarget;

	copyToFrameBuff = new PostEffect(data);
}

RenderTargetMgr::~RenderTargetMgr()
{
	MyFunc::SafetyDelete(mainRenderTarget);
	MyFunc::SafetyDelete(depthMapRenderTarget);
	MyFunc::SafetyDelete(clippingRenderTarget);
	MyFunc::SafetyDelete(copyToFrameBuff);
}

void RenderTargetMgr::PreRenderTarget()
{
	this->ClearDepthStencil();

	//表示状態から描画状態に変更
	mainRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	depthMapRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	clippingRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);

	//メインレンダーターゲットのセット
	RenderTarget* targets[] = { mainRenderTarget,depthMapRenderTarget,clippingRenderTarget };
	RenderTarget::SetRenderTarget(targets, _countof(targets));

	//レンダーターゲットのクリア
	mainRenderTarget->Clear();
	depthMapRenderTarget->Clear();
	clippingRenderTarget->Clear();

	recentRenderTarget = mainRenderTarget;
}

void RenderTargetMgr::ClearDepthStencil()
{
	D3D12App::Instance()->GetCmdList()->ClearDepthStencilView(
		dsvCpuHandle,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr);
}

void RenderTargetMgr::CopyResultToFrameBuff()
{
	//フレームバッファレンダーターゲットをセット
	D3D12App::Instance()->SetFrameRenderTarget();
	copyToFrameBuff->ExcutePostEffect(recentRenderTarget);
}

void RenderTargetMgr::ResetTargets()
{
	//表示状態から描画状態に変更
	recentRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	depthMapRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	clippingRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);

	//メインレンダーターゲットのセット（最新のカラーレンダーターゲットがセットされる）
	RenderTarget* targets[] = { recentRenderTarget,depthMapRenderTarget,clippingRenderTarget };
	RenderTarget::SetRenderTarget(targets, _countof(targets));
}
