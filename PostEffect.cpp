#include "PostEffect.h"
#include"D3D12App.h"
#include"WinApp.h"
#include"PipelineMgr.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include"SRVDescMgr.h"
#include"RenderTarget.h"
#include<iterator>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

bool PostEffect::SHARE_DATA_IS_INIT = false;
ComPtr<ID3D12Resource> PostEffect::VERT_BUFF = nullptr;
D3D12_VERTEX_BUFFER_VIEW PostEffect::VB_VIEW;

void PostEffect::InitShareData()
{
	//float halfWidth = WinApp::WIN_WIDTH / 2;
	//float halfHeight = WinApp::WIN_HEIGHT / 2;


	//頂点バッファ生成
	auto hr = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC(CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * 4)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&VERT_BUFF));

	//頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	HRESULT result = VERT_BUFF->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0; i < 4; ++i)
	{
		vertMap[0] = { {-1.0f,-1.0f},{0.0f,1.0f} };	//左下
		vertMap[1] = { {-1.0f,1.0f},{0.0f,0.0f} };	//左上
		vertMap[2] = { {1.0f,-1.0f},{1.0f,1.0f} };	//右下
		vertMap[3] = { {1.0f,1.0f},{1.0f,0.0f} };	//右上
	}
	VERT_BUFF->Unmap(0, nullptr);

	//頂点バッファビューの作成
	VB_VIEW.BufferLocation = VERT_BUFF->GetGPUVirtualAddress();
	VB_VIEW.SizeInBytes = sizeof(Vertex) * 4;
	VB_VIEW.StrideInBytes = sizeof(Vertex);

	SHARE_DATA_IS_INIT = true;
}

void PostEffect::CreateRootSignature(PostEffectInitData InitData, ID3D12RootSignature** RootSignature)
{
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	std::vector<CD3DX12_ROOT_PARAMETER> rootParameters;
	std::vector< CD3DX12_DESCRIPTOR_RANGE> ranges;

	/*
		事前にreserveで使う領域を確保しておく。
		これがないとemplace_backのタイミングで別の場所に配列をまるごと移動するが、
		rangeのポインタのアドレスは移動後の跡地を見てしまうため、
		そのまるごと移動が起こらないようにする。
	*/
	ranges.reserve(InitData.refferRenderTarget.size() + InitData.additionalSRVHandle.size());

	int total = 0;
	int srvNum = 0;
	//参照するレンダーターゲット
	while (InitData.refferRenderTarget[srvNum] != nullptr)
	{
		ranges.emplace_back();
		ranges[srvNum].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, srvNum);

		rootParameters.emplace_back();
		rootParameters[total].InitAsDescriptorTable(1, &ranges[srvNum], D3D12_SHADER_VISIBILITY_ALL);

		++total;
		++srvNum;
	}
	//追加SRV用
	for (int index = 0; index < InitData.additionalSRVHandle.size(); ++index)
	{
		ranges.emplace_back();
		ranges[srvNum].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, srvNum);

		rootParameters.emplace_back();
		rootParameters[total].InitAsDescriptorTable(1, &ranges[srvNum], D3D12_SHADER_VISIBILITY_ALL);

		++total;
		++srvNum;
	}
	//追加定数バッファ用
	for (int index = 0; index < InitData.additionalConstBuff.size(); ++index)
	{
		rootParameters.emplace_back();
		rootParameters[total].InitAsConstantBufferView(index, 0, D3D12_SHADER_VISIBILITY_ALL);

		++total;
	}

	// Allow input layout and deny uneccessary access to certain pipeline stages.
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(rootParameters.size(), &rootParameters[0], 1, &sampler, rootSignatureFlags);

	Microsoft::WRL::ComPtr<ID3DBlob> signature = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
	auto hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	
	//シェーダのエラー内容を表示
	if (FAILED(hr))
	{
		//errorBlobからエラー内容string型にコピー
		std::string errstr;
		errstr.resize(error->GetBufferSize());

		std::copy_n((char*)error->GetBufferPointer(),
			error->GetBufferSize(),
			errstr.begin());
		errstr += '\n';
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
	hr = D3D12App::Instance()->GetDev()->
		CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&(*RootSignature)));
	if (FAILED(hr)) {
		//ルートシグネチャの作成に失敗した。
		DebugBreak();
	}
}

void PostEffect::CreatePipelineState(PostEffectInitData InitData, 
	ID3D12RootSignature* RootSignature, ID3D12PipelineState** PipelineState)
{
	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC input[] =
	{
		//xyz座標
		{
			"POSITION",		//セマンティック名
			0,				//同じセマンティック名が複数あるときに使うインデックス（０でよい）
			DXGI_FORMAT_R32G32_FLOAT,	//要素数とビット数を表す（XYZの３つでfloat型なので R32G32B32_FLOAT)
			0,	//入力スロットインデックス（０でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,	//データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0		//一度に描画するインスタンス数（０でよい）
		},
		//uv座標
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	ComPtr<ID3DBlob> vertexBlob = nullptr;	//頂点シェーダオブジェクト
	ComPtr<ID3DBlob> pixelBlob = nullptr;	//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr;	//エラーオブジェクト

	HRESULT result;

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		InitData.vertexShader.c_str(),		//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "vs_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&vertexBlob, &errorBlob);
	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		InitData.pixelShader.c_str(),		//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "ps_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&pixelBlob, &errorBlob);
	//シェーダのエラー内容を表示
	if (FAILED(result))
	{
		DebugBreak();
	}

	//パイプラインステートを作成。
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { input, _countof(input) };
	psoDesc.pRootSignature = RootSignature;
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexBlob.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelBlob.Get());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	if (InitData.alphaBlendMode == PostEffectInfo::AlphaBlendMode_Trans) 
	{
		//半透明合成のブレンドステートを作成する。
		psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	}
	else if (InitData.alphaBlendMode == PostEffectInfo::AlphaBlendMode_Add) 
	{
		//加算合成。
		psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	}

	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	for (auto& format : InitData.colorBufferFormat) {
		if (format == DXGI_FORMAT_UNKNOWN) {
			break;
		}
		psoDesc.RTVFormats[psoDesc.NumRenderTargets] = format;
		psoDesc.NumRenderTargets++;
	}

	psoDesc.BlendState.IndependentBlendEnable = true;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	auto hr = D3D12App::Instance()->GetDev()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&(*PipelineState)));
	if (FAILED(hr)) {
		DebugBreak();
	}
}

PostEffect::PostEffect(PostEffectInitData InitData)
{
	name = InitData.name;
	//共有データの初期化
	if (!SHARE_DATA_IS_INIT)
	{
		InitShareData();
	}

	if (!PipelineMgr::Instance()->Exsit(name))
	{
		//ルートシグネチャ
		ID3D12RootSignature* rootSignature = nullptr;
		CreateRootSignature(InitData,&rootSignature);
		//パイプラインステート
		ID3D12PipelineState* pipelineState = nullptr;
		CreatePipelineState(InitData, rootSignature, &pipelineState);

		//パイプラインマネージャに委託
		PipelineMgr::Instance()->AddPipeLine(name, rootSignature, pipelineState, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	this->refferRenderTarget = InitData.refferRenderTarget;

	//データ配列のムーブ
	std::move(InitData.additionalSRVHandle.begin(), InitData.additionalSRVHandle.end(), std::back_inserter(this->additionalSRVHandle));
	std::move(InitData.additionalConstBuff.begin(), InitData.additionalConstBuff.end(), std::back_inserter(this->additionalConstBuff));
}

void PostEffect::ExcutePostEffect()
{
	//ルートシグネチャ情報がパイプラインマネージャから離れる
	//PipelineMgr::Instance()->Reset();

	PipelineMgr::Instance()->SetPipePine(name);
	//頂点バッファビュー
	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &VB_VIEW);

	int i = 0;
	//レンダーターゲットバッファをテクスチャとして扱う
	while (refferRenderTarget[i] != nullptr)
	{
		(*refferRenderTarget[i])->ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		SRVDescMgr::Instance()->SetSRV((*refferRenderTarget[i])->GetSRVHandle(), i);
		++i;
	}
	//追加分のテクスチャをセット
	for (int index = 0; index < additionalSRVHandle.size(); ++index)
	{
		SRVDescMgr::Instance()->SetSRV(*additionalSRVHandle[index], i);
		++i;
	}
	//追加分の定数バッファ
	for (int index = 0; index < additionalConstBuff.size(); ++index)
	{
		D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(i, additionalConstBuff[index]->GetGPUVirtualAddress());
		++i;
	}
	//描画
	D3D12App::Instance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::ExcutePostEffect(RenderTarget* ResourceToCopy)
{
	PipelineMgr::Instance()->SetPipePine(name);

	//頂点バッファビュー
	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &VB_VIEW);

	ResourceToCopy->ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	SRVDescMgr::Instance()->SetSRV(ResourceToCopy->GetSRVHandle(), 0);

	D3D12App::Instance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}
