#pragma once
#include <d3d12.h>
#include<wrl.h>

class Pipeline
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature>rootSignature;	//ルートシグネチャ
	ComPtr<ID3D12PipelineState>pipeLine;			//パイプラインステート
	D3D_PRIMITIVE_TOPOLOGY topology;	//Draw前にセットするトポロジー

	void CompileShaders(
		const wchar_t* VSName, ID3DBlob** VS,
		const wchar_t* PSName, ID3DBlob** PS,
		const wchar_t* GSName = nullptr, ID3DBlob** GS = nullptr);

	void SetPipelineState(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* Gpipeline,
		D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
		D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
		D3D_PRIMITIVE_TOPOLOGY Topology, bool CallMode, bool DepthWriteMask,
		D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler,
		bool DepthTest);

public:
	Pipeline() {};
	Pipeline(
		const wchar_t* VSName, const wchar_t* PSName,
		D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
		D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
		D3D_PRIMITIVE_TOPOLOGY Topology, bool CallMode, bool DepthWriteMask,
		D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler,
		bool DepthTest);
	Pipeline(
		const wchar_t* VSName, const wchar_t* PSName, const wchar_t* GSName,
		D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
		D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
		D3D_PRIMITIVE_TOPOLOGY Topology, bool CallMode, bool DepthWriteMask,
		D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler,
		bool DepthTest);
	Pipeline(ID3D12RootSignature* RootSignature,
		ID3D12PipelineState* PipeLine, D3D_PRIMITIVE_TOPOLOGY Topology);

	void SetPipeLine();
};