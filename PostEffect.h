#pragma once
#include<d3d12.h>
#include<wrl.h>
#include"MyFunc.h"
using namespace MyDef;
#include"DescHeap.h"
#include<array>
#include<string>
#include<vector>

class RenderTarget;

namespace PostEffectInfo
{
	enum AlphaBlendMode
	{
		AlphaBlendMode_None,	//�A���t�@�u�����f�B���O�Ȃ�(�㏑��)�B
		AlphaBlendMode_Trans,	//����������
		AlphaBlendMode_Add,		//���Z����
	};

	//�ő�R�Q���̃e�N�X�`�����g�p�i���̃����_�[�^�[�Q�b�g�o�b�t�@���e�N�X�`���Ƃ��Ĉ��������Ă���j
	static const int MAX_TEXTURE = 32;
}

struct PostEffectInitData
{
	//�|�X�g�G�t�F�N�g��
	std::string name;

	//���_�V�F�[�_���O
	std::wstring vertexShader = L"NonPostEffectVertexShader.hlsl";
	//�s�N�Z���V�F�[�_���O
	std::wstring pixelShader = L"NonPostEffectPixelShader.hlsl";

	//�Q�Ƃ��郌���_�[�^�[�Q�b�g
	std::array<RenderTarget**, PostEffectInfo::MAX_TEXTURE>refferRenderTarget = { nullptr };

	PostEffectInfo::AlphaBlendMode alphaBlendMode = PostEffectInfo::AlphaBlendMode_None;

	//�������ݐ惌���_�[�^�[�Q�b�g�̃J���[�o�b�t�@�t�H�[�}�b�g�i�p�C�v���C�������ɕK�v�j
	std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>colorBufferFormat =
	{
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
	};

	//�ǉ�SRV�o�b�t�@�̐�
	std::vector<int*> additionalSRVHandle;
	//�ǉ��萔�o�b�t�@�̐�
	std::vector<ID3D12Resource*> additionalConstBuff;
};

class PostEffect
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	//���L�f�[�^�̏������͍ς�ł��邩
	static bool SHARE_DATA_IS_INIT;

	static void InitShareData();

	struct Vertex
	{
		float2 pos;
		float2 uv;
	};
	//���LVertex�f�[�^
	static ComPtr<ID3D12Resource> VERT_BUFF;	//���_�o�b�t�@
	static D3D12_VERTEX_BUFFER_VIEW VB_VIEW;			//���_�o�b�t�@�r���[

	//�|�X�g�G�t�F�N�g��
	std::string name;
	//�Q�Ƃ��郌���_�[�^�[�Q�b�g�o�b�t�@
	std::array<RenderTarget**, PostEffectInfo::MAX_TEXTURE>refferRenderTarget = { nullptr };
	//�ǉ�SRV�o�b�t�@�̐�
	std::vector<int*> additionalSRVHandle;
	//�ǉ��萔�o�b�t�@�̐�
	std::vector<ID3D12Resource*> additionalConstBuff;

	void CreateRootSignature(PostEffectInitData InitData, ID3D12RootSignature** RootSignature);
	void CreatePipelineState(PostEffectInitData InitData, 
		ID3D12RootSignature* RootSignature,ID3D12PipelineState** PipelineState);

public:
	PostEffect(PostEffectInitData InitData);

	void ExcutePostEffect();
	void ExcutePostEffect(RenderTarget* ResourceToCopy);
};