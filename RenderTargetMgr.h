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

	//���ꂵ�Ďg���f�v�X�X�e���V���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>depthStencil = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle;

	//�V�[���̃J���[��`�����ރJ���[�����_�����O�^�[�Q�b�g(���H���郌���_�[�^�[�Q�b�g)
	RenderTarget* mainRenderTarget = nullptr;
	//�V�[���̃J������Ԃł�Z�l���������ރ����_�����O�^�[�Q�b�g(�[�x�}�b�v)
	RenderTarget* depthMapRenderTarget = nullptr;
	//�N���b�s���O�p�����_�[�^�[�Q�b�g(�`�����܂ꂽ�s�N�Z���̃���0�łȂ��Ƃ��A�O���P�̂Qbit)
	RenderTarget* clippingRenderTarget = nullptr;

	//�ŐV�̃J���[�����_�[�^�[�Q�b�g
	RenderTarget* recentRenderTarget = nullptr;

	PostEffect* copyToFrameBuff = nullptr;

	void CreateDepthStencil();
	RenderTargetMgr();

public:
	~RenderTargetMgr();

	//���C����Z�o�b�t�@�����_�[�^�[�Q�b�g�ւ̕`�����ݏ���
	void PreRenderTarget();

	//�f�v�X�X�e���V�����N���A
	void ClearDepthStencil();
	//Z�o�b�t�@�p�����_�[�^�[�Q�b�g���N���A
	void ClearDepthMapRenderTarget() { depthMapRenderTarget->Clear(); }
	//�N���b�s���O�p�����_�[�^�[�Q�b�g���N���A
	void ClearClippingRenderTarget() { clippingRenderTarget->Clear(); }

	//�ŐV�����_�[�^�[�Q�b�g���t���[���o�b�t�@�ւ̕`������
	void CopyResultToFrameBuff();

	//�ŐV�̃����_�[�^�[�Q�b�g�̃|�C���^�Q�b�^
	RenderTarget** GetRecentRenderTarget() { return &recentRenderTarget; }
	//Z�o�b�t�@�����_�[�^�[�Q�b�g�̃|�C���^�Q�b�^
	RenderTarget* GetZBuffRenderTarget() { return depthMapRenderTarget; }
	//�N���b�s���O�p�����_�[�^�[�Q�b�g�̃|�C���^�Q�b�^
	RenderTarget** GetClipRenderTarget() { return &clippingRenderTarget; }

	//�ŐV�̃����_�[�^�[�Q�b�g�Ƃ��ăZ�b�g
	void SetRecentRenderTarget(RenderTarget* RenderTarget) { recentRenderTarget = RenderTarget; }

	//���C���̃f�v�X�X�e���V���̃n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDepthStencil() { return &dsvCpuHandle; }

	//recent�����_�[�^�[�Q�b�g��Z�o�b�t�@�����_�[�^�[�Q�b�g���ĂуZ�b�g
	void ResetTargets();
};

