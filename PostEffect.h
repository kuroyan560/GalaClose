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
		AlphaBlendMode_None,	//アルファブレンディングなし(上書き)。
		AlphaBlendMode_Trans,	//半透明合成
		AlphaBlendMode_Add,		//加算合成
	};

	//最大３２枚のテクスチャを使用可（他のレンダーターゲットバッファをテクスチャとして引っ張ってくる）
	static const int MAX_TEXTURE = 32;
}

struct PostEffectInitData
{
	//ポストエフェクト名
	std::string name;

	//頂点シェーダ名前
	std::wstring vertexShader = L"NonPostEffectVertexShader.hlsl";
	//ピクセルシェーダ名前
	std::wstring pixelShader = L"NonPostEffectPixelShader.hlsl";

	//参照するレンダーターゲット
	std::array<RenderTarget**, PostEffectInfo::MAX_TEXTURE>refferRenderTarget = { nullptr };

	PostEffectInfo::AlphaBlendMode alphaBlendMode = PostEffectInfo::AlphaBlendMode_None;

	//書き込み先レンダーターゲットのカラーバッファフォーマット（パイプライン生成に必要）
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

	//追加SRVバッファの数
	std::vector<int*> additionalSRVHandle;
	//追加定数バッファの数
	std::vector<ID3D12Resource*> additionalConstBuff;
};

class PostEffect
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	//共有データの初期化は済んでいるか
	static bool SHARE_DATA_IS_INIT;

	static void InitShareData();

	struct Vertex
	{
		float2 pos;
		float2 uv;
	};
	//共有Vertexデータ
	static ComPtr<ID3D12Resource> VERT_BUFF;	//頂点バッファ
	static D3D12_VERTEX_BUFFER_VIEW VB_VIEW;			//頂点バッファビュー

	//ポストエフェクト名
	std::string name;
	//参照するレンダーターゲットバッファ
	std::array<RenderTarget**, PostEffectInfo::MAX_TEXTURE>refferRenderTarget = { nullptr };
	//追加SRVバッファの数
	std::vector<int*> additionalSRVHandle;
	//追加定数バッファの数
	std::vector<ID3D12Resource*> additionalConstBuff;

	void CreateRootSignature(PostEffectInitData InitData, ID3D12RootSignature** RootSignature);
	void CreatePipelineState(PostEffectInitData InitData, 
		ID3D12RootSignature* RootSignature,ID3D12PipelineState** PipelineState);

public:
	PostEffect(PostEffectInitData InitData);

	void ExcutePostEffect();
	void ExcutePostEffect(RenderTarget* ResourceToCopy);
};