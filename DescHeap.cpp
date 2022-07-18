#include "DescHeap.h"
#include"D3D12App.h"

std::vector<ID3D12DescriptorHeap*>DescHeap::HEAPS;

const int DescHeap::MAX_DESCRIPTOR_NUM[TYPE_NUM] =
{
    2048,100,2048,100
};

void DescHeap::SetHeaps()
{
    if (!HEAPS.empty())
    {
        //デスクリ-プタヒープをセット
        D3D12App::Instance()->GetCmdList()->SetDescriptorHeaps(HEAPS.size(), &HEAPS[0]);
    }
}

DescHeap::DescHeap(DescType Type, ID3D12Device* Device)
{
    HRESULT result;

    D3D12_DESCRIPTOR_HEAP_FLAGS flag = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    if (Type == RTV || Type == DSV)
    {
        flag = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    }

    // SRV_CBV_UAV のディスクリプタヒープ
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{
      (D3D12_DESCRIPTOR_HEAP_TYPE)Type,
      MAX_DESCRIPTOR_NUM[Type],
      flag,
      0
    };
    result = Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
    headHandleCpu = heap.Get()->GetCPUDescriptorHandleForHeapStart();
    headHandleGpu = heap.Get()->GetGPUDescriptorHandleForHeapStart();
    incrementSize = Device->GetDescriptorHandleIncrementSize(heapDesc.Type);

    if (Type != RTV && Type != DSV)
    {
        HEAPS.push_back(heap.Get());
    }
}