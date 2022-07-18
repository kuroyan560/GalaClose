#include "PipelineMgr.h"

PipelineMgr::~PipelineMgr()
{
	pipelines.clear();
}

void PipelineMgr::AddPipeLine(const string Key,
	const wchar_t* VSName, const wchar_t* PSName,
	D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
	D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
	D3D_PRIMITIVE_TOPOLOGY Topology,
	bool CallMode, bool DepthWriteMask,
	D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler,
	bool DepthTest)
{
	if (Exsit(Key))
	{
		return;
	}

	pipelines[Key] = Pipeline(
		VSName, PSName,
		InputLayout, NumInputLayout,
		RootParams, NumRootParams,
		Topology,
		CallMode, DepthWriteMask,
		StaticSampler, NumStaticSampler,
		DepthTest);
}

void PipelineMgr::AddPipeLine(string Key,
	const wchar_t* VSName, const wchar_t* PSName, const wchar_t* GSName,
	D3D12_INPUT_ELEMENT_DESC* InputLayout, UINT NumInputLayout,
	D3D12_ROOT_PARAMETER* RootParams, UINT NumRootParams,
	D3D_PRIMITIVE_TOPOLOGY Topology,
	bool CallMode, bool DepthWriteMask,
	D3D12_STATIC_SAMPLER_DESC* StaticSampler, UINT NumStaticSampler,
	bool DepthTest)
{
	if (Exsit(Key))
	{
		return;
	}

	pipelines[Key] = Pipeline(
		VSName, PSName, GSName,
		InputLayout, NumInputLayout,
		RootParams, NumRootParams,
		Topology,
		CallMode, DepthWriteMask,
		StaticSampler, NumStaticSampler,
		DepthTest);
}

void PipelineMgr::AddPipeLine(string Key, ID3D12RootSignature* RootSignature, ID3D12PipelineState* PipeLine, D3D_PRIMITIVE_TOPOLOGY Topology)
{
	if (Exsit(Key))
	{
		return;
	}

	pipelines[Key] = Pipeline(RootSignature, PipeLine, Topology);
}

void PipelineMgr::SetPipePine(string Key)
{
	if (nowPipeline != Key)
	{
		pipelines[Key].SetPipeLine();
		nowPipeline = Key;
	}
}
