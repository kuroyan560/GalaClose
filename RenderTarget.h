#pragma once
#include<wrl.h>
#include<d3dx12.h>
#include"WinApp.h"

class RenderTarget
{
public:
	//�����_�[�^�[�Q�b�g���Z�b�g
	static void SetRenderTarget(RenderTarget** RenderTargetArray, int Num);

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	//�����_�[�^�[�Q�b�g�o�b�t�@
	ComPtr<ID3D12Resource>renderTarget = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle;
	D3D12_RESOURCE_STATES rtvCurrentState;//���\�[�X�o���A�̏��
	//�����_�[�^�[�Q�b�g�e�N�X�`���r���[�̃n���h��(SRV)
	int srvHandle = -1;


	float rtvClearValue[4] = { 0.0f,0.0f,0.0f,1.0f };

	void CreateRenderTarget(DXGI_FORMAT ColorFormat, float* ClearValue, int Width, int Height, int MipLevel, int ArraySize);

public:
	RenderTarget(DXGI_FORMAT ColorFormat, float* ClearValue = nullptr,
		int Width = WinApp::WIN_WIDTH, int Height = WinApp::WIN_HEIGHT, int MipLevel = 1, int ArraySize = 1);

	//���\�[�X�o���A�̕ύX
	void ChangeResourceBarrier(D3D12_RESOURCE_STATES State);

	//�����_�[�^�[�Q�b�g�̃N���A
	void Clear();

	ID3D12Resource* GetRenderTarget() { return renderTarget.Get(); }

	CD3DX12_CPU_DESCRIPTOR_HANDLE* GetRTVCpuHandle() { return &rtvCpuHandle; }

	//�����_�[�^�[�Q�b�g�e�N�X�`���Ƃ��Ďg�p
	int GetSRVHandle();
};

