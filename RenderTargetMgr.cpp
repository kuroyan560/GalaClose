#include "RenderTargetMgr.h"
#include"MyFunc.h"
#include"D3D12App.h"
#include"RTV_DSVDescMgr.h"

void RenderTargetMgr::CreateDepthStencil()
{
	CD3DX12_RESOURCE_DESC depthResDec = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,	//�[�x�l�t�H�[�}�b�g
		static_cast<UINT>(WinApp::WIN_WIDTH),	
		static_cast<UINT>(WinApp::WIN_HEIGHT),
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);	//�f�v�X�X�e���V��

	auto hr = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDec,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthStencil));

	if (FAILED(hr)) {
		//�[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s�B
		DebugBreak();
	}

	int handle = RTV_DSVDescMgr::Instance()->CreateDSV(depthStencil.Get());
	dsvCpuHandle = RTV_DSVDescMgr::Instance()->GetCpuHandleDSV(handle);
}

RenderTargetMgr::RenderTargetMgr()
{
	CreateDepthStencil();

	mainRenderTarget = new RenderTarget(DXGI_FORMAT_R8G8B8A8_UNORM, mainClearValue);//�J���[�}�b�v
	depthMapRenderTarget = new RenderTarget(DXGI_FORMAT_R32_FLOAT); //�[�x�}�b�v
	clippingRenderTarget = new RenderTarget(DXGI_FORMAT_R32_FLOAT);//�N���b�s���O�p(0��1)

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

	//�\����Ԃ���`���ԂɕύX
	mainRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	depthMapRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	clippingRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);

	//���C�������_�[�^�[�Q�b�g�̃Z�b�g
	RenderTarget* targets[] = { mainRenderTarget,depthMapRenderTarget,clippingRenderTarget };
	RenderTarget::SetRenderTarget(targets, _countof(targets));

	//�����_�[�^�[�Q�b�g�̃N���A
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
	//�t���[���o�b�t�@�����_�[�^�[�Q�b�g���Z�b�g
	D3D12App::Instance()->SetFrameRenderTarget();
	copyToFrameBuff->ExcutePostEffect(recentRenderTarget);
}

void RenderTargetMgr::ResetTargets()
{
	//�\����Ԃ���`���ԂɕύX
	recentRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	depthMapRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);
	clippingRenderTarget->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET);

	//���C�������_�[�^�[�Q�b�g�̃Z�b�g�i�ŐV�̃J���[�����_�[�^�[�Q�b�g���Z�b�g�����j
	RenderTarget* targets[] = { recentRenderTarget,depthMapRenderTarget,clippingRenderTarget };
	RenderTarget::SetRenderTarget(targets, _countof(targets));
}
