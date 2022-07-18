#pragma once
#include<wrl.h>
#include<d3d12.h>
#include"d3dx12.h"

#include<vector>

enum DescType
{
	CBV_SRV_UAV = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	RTV = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	DSV = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
	TYPE_NUM = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES
};

class DescHeap
{
	static std::vector<ID3D12DescriptorHeap*> HEAPS;
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int MAX_DESCRIPTOR_NUM[TYPE_NUM];
	static const int MAX_SRV_CBV_UAV = 2048;
	static const int MAX_SAMPLER = 100;
	static const int MAX_RTV = 100;
	static const int MAX_DSV = 100;

	static void SetHeaps();
private:
	ComPtr<ID3D12DescriptorHeap>heap;

	D3D12_CPU_DESCRIPTOR_HANDLE headHandleCpu;
	D3D12_GPU_DESCRIPTOR_HANDLE headHandleGpu;
	UINT incrementSize;

	int num = 0;	//生成済ディスクリプタの数

public:
	DescHeap(DescType Type, ID3D12Device* Device);
	int GetDescNum() { return num; }
	void AddDescNum() { num++; }
#pragma region ハンドルゲッタ
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(const int& Index)	//場所指定
	{
		CD3DX12_GPU_DESCRIPTOR_HANDLE handle(headHandleGpu, Index, incrementSize);
		return handle;
	}
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle()	//最後尾
	{
		CD3DX12_GPU_DESCRIPTOR_HANDLE handle(headHandleGpu, num, incrementSize);
		return handle;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(const int& Index)	//場所指定
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle(headHandleCpu, Index, incrementSize);
		return handle;
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle()	//最後尾
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle(headHandleCpu, num, incrementSize);
		return handle;
	}
#pragma endregion
};