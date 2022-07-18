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
	ComPtr<ID3DBlob> errorBlob = nullptr;	//エラーオブジェクト

	HRESULT result;

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		VSName,		//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "vs_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&*VS, &errorBlob);
	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		PSName,		//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "ps_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&*PS, &errorBlob);

	//ジオメトリシェーダの読み込みとコンパイル
	if (GSName != nullptr && GS != nullptr)
	{
		//ジオメトリシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			GSName,	// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&*GS, &errorBlob);
	}

	//シェーダのエラー内容を表示
	if (FAILED(result))
	{
		//errorBlobからエラー内容string型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += '\n';
		//エラー内容を出力ウィンドウに表示
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

	// サンプルマスク
	Gpipeline->SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	Gpipeline->RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	if (!CallMode)
	{
		Gpipeline->RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	}
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	if (DepthTest)
	{
		Gpipeline->DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		//デプスの書き込みを禁止（深度テストは行う）
		if (!DepthWriteMask)
		{
			Gpipeline->DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
	}

	//同時レンダーターゲットで独立したブレンディングを有効にするか
	Gpipeline->BlendState.IndependentBlendEnable = true;

	//メインレンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc_main{};
	blenddesc_main.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc_main.BlendEnable = true;

	blenddesc_main.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc_main.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc_main.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc_main.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc_main.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc_main.DestBlendAlpha = D3D12_BLEND_ZERO;

	//Zバッファ描き込み用レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc_depth{};
	blenddesc_depth.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_RED;
	blenddesc_depth.BlendEnable = false;
	
	blenddesc_depth.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc_depth.SrcBlend = D3D12_BLEND_ONE;
	blenddesc_depth.DestBlend = D3D12_BLEND_ZERO;

	blenddesc_depth.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc_depth.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc_depth.DestBlendAlpha = D3D12_BLEND_ZERO;

	//クリッピング用レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc_clip{};
	blenddesc_clip.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_RED;
	blenddesc_clip.BlendEnable = true;

	blenddesc_clip.BlendOp = D3D12_BLEND_OP_MAX;
	blenddesc_clip.SrcBlend = D3D12_BLEND_ONE;
	blenddesc_clip.DestBlend = D3D12_BLEND_ONE;

	blenddesc_clip.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc_clip.SrcBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc_clip.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	Gpipeline->BlendState.RenderTarget[0] = blenddesc_main;//カラー
	Gpipeline->BlendState.RenderTarget[1] = blenddesc_depth;//深度マップ
	Gpipeline->BlendState.RenderTarget[2] = blenddesc_clip;	//クリッピング用

	// 深度バッファのフォーマット
	Gpipeline->DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	Gpipeline->InputLayout.pInputElementDescs = InputLayout;
	Gpipeline->InputLayout.NumElements = NumInputLayout;

	// 図形の形状設定（三角形）
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

	Gpipeline->NumRenderTargets = 3;	// 描画対象は2つ
	//メインレンダーターゲットのフォーマット
	Gpipeline->RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Zバッファ描き込み用レンダーターゲットのフォーマット
	Gpipeline->RTVFormats[1] = DXGI_FORMAT_R32_FLOAT;
	//クリッピング用レンダーターゲットのフォーマット
	Gpipeline->RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;
	Gpipeline->SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// スタティックサンプラー
	int samplerNum = NumStaticSampler;
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	if (StaticSampler == nullptr)
	{
		StaticSampler = &samplerDesc;
		samplerNum = 1;
	}
	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(NumRootParams, RootParams, samplerNum, StaticSampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob = nullptr;	//エラーオブジェクト
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = D3D12App::Instance()->GetDev()->
		CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	Gpipeline->pRootSignature = rootSignature.Get();

	// グラフィックスパイプラインの生成
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
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	CompileShaders(VSName, vsBlob.GetAddressOf(), PSName, psBlob.GetAddressOf());

	// グラフィックスパイプラインの流れを設定
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
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	//ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	CompileShaders(VSName, vsBlob.GetAddressOf(), PSName, psBlob.GetAddressOf(), GSName, gsBlob.GetAddressOf());

	// グラフィックスパイプラインの流れを設定
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
	//パイプラインステートの設定
	D3D12App::Instance()->GetCmdList()->SetPipelineState(pipeLine.Get());
	//ルートシグネチャの設定
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状を設定
	D3D12App::Instance()->GetCmdList()->IASetPrimitiveTopology(topology);
	//D3D12App::Instance()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
}