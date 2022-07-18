#pragma once
#include"Singleton.h"
#include"PostEffect.h"
#include<queue>
#include"RenderTarget.h"
#include"DescHeap.h"
#include<wrl.h>
#include"src/users/Constants.h"

class RenderTargetMgr : public Singleton<RenderTargetMgr>
{
	friend class Singleton<RenderTargetMgr>;

	float mainClearValue[4] = { BACK_GROUND_COLOR[0],BACK_GROUND_COLOR[1],BACK_GROUND_COLOR[2],1.0f };

	//統一して使うデプスステンシルバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>depthStencil = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle;

	//シーンのカラーを描きこむカラーレンダリングターゲット(加工するレンダーターゲット)
	RenderTarget* mainRenderTarget = nullptr;
	//シーンのカメラ空間でのZ値を書きこむレンダリングターゲット(深度マップ)
	RenderTarget* depthMapRenderTarget = nullptr;
	//クリッピング用レンダーターゲット(描き込まれたピクセルのαが0でないとき、０か１の２bit)
	RenderTarget* clippingRenderTarget = nullptr;

	//最新のカラーレンダーターゲット
	RenderTarget* recentRenderTarget = nullptr;

	PostEffect* copyToFrameBuff = nullptr;

	void CreateDepthStencil();
	RenderTargetMgr();

public:
	~RenderTargetMgr();

	//メインとZバッファレンダーターゲットへの描き込み準備
	void PreRenderTarget();

	//デプスステンシルをクリア
	void ClearDepthStencil();
	//Zバッファ用レンダーターゲットをクリア
	void ClearDepthMapRenderTarget() { depthMapRenderTarget->Clear(); }
	//クリッピング用レンダーターゲットをクリア
	void ClearClippingRenderTarget() { clippingRenderTarget->Clear(); }

	//最新レンダーターゲットをフレームバッファへの描き込み
	void CopyResultToFrameBuff();

	//最新のレンダーターゲットのポインタゲッタ
	RenderTarget** GetRecentRenderTarget() { return &recentRenderTarget; }
	//Zバッファレンダーターゲットのポインタゲッタ
	RenderTarget* GetZBuffRenderTarget() { return depthMapRenderTarget; }
	//クリッピング用レンダーターゲットのポインタゲッタ
	RenderTarget** GetClipRenderTarget() { return &clippingRenderTarget; }

	//最新のレンダーターゲットとしてセット
	void SetRecentRenderTarget(RenderTarget* RenderTarget) { recentRenderTarget = RenderTarget; }

	//メインのデプスステンシルのハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDepthStencil() { return &dsvCpuHandle; }

	//recentレンダーターゲットとZバッファレンダーターゲットを再びセット
	void ResetTargets();
};

