#include "Pipeline.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<string>
#include "d3dx12.h"

#include"D3D12App.h"

void Pipeline::CompileShaders(
	const wchar_t* VSName, ID3DBlob** VS, 
	const wchar_t* PSName, ID3DBlob** PS, 
	const wchar_t* GSName, ID3DBlob** GS)
{
	ComPtr<ID3DBlob> errorBlob = nullptr;	//�G���[�I�u�W�F�N�g

	HRESULT result;

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		VSName,		//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main", "vs_5_0",	//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�O�p�ݒ�
		0,
		&*VS, &errorBlob);
	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		PSName,		//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main", "ps_5_0",	//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�O�p�ݒ�
		0,
		&*PS, &errorBlob);

	//�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	if (GSName != nullptr && GS != nullptr)
	{
		//�W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
		result = D3DCompileFromFile(
			GSName,	// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
			"main", "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
			0,
			&*GS, &errorBlob);
	}

	//�V�F�[�_�̃G���[���e��\��
	if (FAILED(result))
	{
		//errorBlob����G���[���estring�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += '\n';
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void Pipeline::SetPipelineState(
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* Gpipeline,
	D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
	D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
	D3D_PRIMITIVE_TOPOLOGY Topology, bool CallMode, bool DepthWriteMask,
	D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler, bool DepthTest)
{
	HRESULT result;

	// �T���v���}�X�N
	Gpipeline->SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	Gpipeline->RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	if (!CallMode)
	{
		Gpipeline->RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	if (DepthTest)
	{
		Gpipeline->DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		//�f�v�X�̏������݂��֎~�i�[�x�e�X�g�͍s���j
		if (!DepthWriteMask)
		{
			Gpipeline->DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
	}

	//���������_�[�^�[�Q�b�g�œƗ������u�����f�B���O��L���ɂ��邩
	Gpipeline->BlendState.IndependentBlendEnable = true;

	//���C�������_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc_main{};
	blenddesc_main.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc_main.BlendEnable = true;

	blenddesc_main.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc_main.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc_main.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc_main.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc_main.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc_main.DestBlendAlpha = D3D12_BLEND_ZERO;

	//Z�o�b�t�@�`�����ݗp�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc_depth{};
	blenddesc_depth.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_RED;
	blenddesc_depth.BlendEnable = false;
	
	blenddesc_depth.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc_depth.SrcBlend = D3D12_BLEND_ONE;
	blenddesc_depth.DestBlend = D3D12_BLEND_ZERO;

	blenddesc_depth.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc_depth.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc_depth.DestBlendAlpha = D3D12_BLEND_ZERO;

	//�N���b�s���O�p�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc_clip{};
	blenddesc_clip.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_RED;
	blenddesc_clip.BlendEnable = true;

	blenddesc_clip.BlendOp = D3D12_BLEND_OP_MAX;
	blenddesc_clip.SrcBlend = D3D12_BLEND_ONE;
	blenddesc_clip.DestBlend = D3D12_BLEND_ONE;

	blenddesc_clip.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc_clip.SrcBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc_clip.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	Gpipeline->BlendState.RenderTarget[0] = blenddesc_main;//�J���[
	Gpipeline->BlendState.RenderTarget[1] = blenddesc_depth;//�[�x�}�b�v
	Gpipeline->BlendState.RenderTarget[2] = blenddesc_clip;	//�N���b�s���O�p

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	Gpipeline->DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	Gpipeline->InputLayout.pInputElementDescs = InputLayout;
	Gpipeline->InputLayout.NumElements = NumInputLayout;

	// �}�`�̌`��ݒ�i�O�p�`�j
	if (Topology == D3D_PRIMITIVE_TOPOLOGY_POINTLIST)
	{
		Gpipeline->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}
	else if (Topology == D3D_PRIMITIVE_TOPOLOGY_LINESTRIP)
	{
		Gpipeline->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	}
	else
	{
		Gpipeline->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}
	//Gpipeline->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	Gpipeline->NumRenderTargets = 3;	// �`��Ώۂ�2��
	//���C�������_�[�^�[�Q�b�g�̃t�H�[�}�b�g
	Gpipeline->RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Z�o�b�t�@�`�����ݗp�����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
	Gpipeline->RTVFormats[1] = DXGI_FORMAT_R32_FLOAT;
	//�N���b�s���O�p�����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
	Gpipeline->RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;
	Gpipeline->SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �X�^�e�B�b�N�T���v���[
	int samplerNum = NumStaticSampler;
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	if (StaticSampler == nullptr)
	{
		StaticSampler = &samplerDesc;
		samplerNum = 1;
	}
	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(NumRootParams, RootParams, samplerNum, StaticSampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob = nullptr;	//�G���[�I�u�W�F�N�g
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = D3D12App::Instance()->GetDev()->
		CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	Gpipeline->pRootSignature = rootSignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = D3D12App::Instance()->GetDev()->CreateGraphicsPipelineState(Gpipeline, IID_PPV_ARGS(&pipeLine));
}

Pipeline::Pipeline(
	const wchar_t* VSName, const wchar_t* PSName,
	D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
	D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
	D3D_PRIMITIVE_TOPOLOGY Topology, bool CallMode,bool DepthWriteMask,
	D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler,
	bool DepthTest) :topology(Topology)
{
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	CompileShaders(VSName, vsBlob.GetAddressOf(), PSName, psBlob.GetAddressOf());

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	SetPipelineState(&gpipeline,
		InputLayout, NumInputLayout,
		RootParams, NumRootParams,
		Topology,CallMode, DepthWriteMask,
		StaticSampler, NumStaticSampler, DepthTest);
}

Pipeline::Pipeline(
	const wchar_t* VSName, const wchar_t* PSName, const wchar_t* GSName,
	D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
	D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
	D3D_PRIMITIVE_TOPOLOGY Topology, bool CallMode,bool DepthWriteMask,
	D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler,
	bool DepthTest) :topology(Topology)
{
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob;	//�W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	CompileShaders(VSName, vsBlob.GetAddressOf(), PSName, psBlob.GetAddressOf(), GSName, gsBlob.GetAddressOf());

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	SetPipelineState(&gpipeline,
		InputLayout, NumInputLayout,
		RootParams, NumRootParams,
		Topology,
		CallMode, DepthWriteMask,
		StaticSampler, NumStaticSampler,DepthTest);
}

Pipeline::Pipeline(ID3D12RootSignature* RootSignature, ID3D12PipelineState* PipeLine, D3D_PRIMITIVE_TOPOLOGY Topology)
{
	rootSignature.Attach(RootSignature);
	pipeLine.Attach(PipeLine);
	topology = Topology;
}

void Pipeline::SetPipeLine()
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	D3D12App::Instance()->GetCmdList()->SetPipelineState(pipeLine.Get());
	//���[�g�V�O�l�`���̐ݒ�
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	D3D12App::Instance()->GetCmdList()->IASetPrimitiveTopology(topology);
	//D3D12App::Instance()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
}