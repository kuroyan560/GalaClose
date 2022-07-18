#pragma once
#include"Singleton.h"

#include<string>
#include<map>

#include"Pipeline.h"
#include<wrl.h>

using namespace std;

class PipelineMgr :public Singleton<PipelineMgr>
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	PipelineMgr() {};
	friend Singleton<PipelineMgr>;

	map<string, Pipeline>pipelines;

	string nowPipeline = "NULL";

public:
	~PipelineMgr();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Key">パイプラインの名前</param>
	/// <param name="VSName">頂点シェーダの名前</param>
	/// <param name="PSName">ピクセルシェーダの名前</param>
	/// <param name="InputLayout">インプットレイアウト</param>
	/// <param name="NumInputLayout">インプットレイアウトの要素数</param>
	/// <param name="RootParams">ルートパラメータ</param>
	/// <param name="NumRootParams">ルートパラメータの要素数</param>
	/// <param name="Topology">描画時のトポロジー指定</param>
	/// <param name="CallMode">背面カリングするか</param>
	/// <param name="DepthWriteMask">デプスの書き込みをするか</param>
	/// <param name="StaticSampler">サンプラー</param>
	/// <param name="NumStaticSampler">サンプラーの要素数</param>
	void AddPipeLine(const string Key,
		const wchar_t* VSName, const wchar_t* PSName,
		D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
		D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
		D3D_PRIMITIVE_TOPOLOGY Topology,
		bool CallMode = true, bool DepthWriteMask = true,
		D3D12_STATIC_SAMPLER_DESC* StaticSampler = nullptr, UINT NumStaticSampler = 0,
		bool DepthTest = true);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="Key">パイプラインの名前</param>
	/// <param name="VSName">頂点シェーダの名前</param>
	/// <param name="PSName">ピクセルシェーダの名前</param>
	/// <param name="GSName">ジオメトリシェーダの名前</param>
	/// <param name="InputLayout">インプットレイアウト</param>
	/// <param name="NumInputLayout">インプットレイアウトの要素数</param>
	/// <param name="RootParams">ルートパラメータ</param>
	/// <param name="NumRootParams">ルートパラメータの要素数</param>
	/// <param name="Topology">描画時のトポロジー指定</param>
	/// <param name="CallMode">背面カリングするか</param>
	/// <param name="DepthWriteMask">デプスの書き込みをするか</param>
	/// <param name="StaticSampler">サンプラー</param>
	/// <param name="NumStaticSampler">サンプラーの要素数</param>
	void AddPipeLine(string Key,
		const wchar_t* VSName, const wchar_t* PSName, const wchar_t* GSName,
		D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
		D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
		D3D_PRIMITIVE_TOPOLOGY Topology,
		bool CallMode = true, bool DepthWriteMask = true,
		D3D12_STATIC_SAMPLER_DESC* StaticSampler = nullptr, UINT NumStaticSampler = 0,
		bool DepthTest = true);
	void AddPipeLine(string Key, ID3D12RootSignature* RootSignature,
		ID3D12PipelineState* PipeLine, D3D_PRIMITIVE_TOPOLOGY Topology);

	void SetPipePine(string Key);

	void Reset() { nowPipeline = "NULL"; }
	bool Exsit(string Key) { return !pipelines.empty() && pipelines.find(Key) != pipelines.end(); }
};