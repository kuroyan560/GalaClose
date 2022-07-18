#include "imguiApp.h"
#include"D3D12App.h"
#include"WinApp.h"
#include"Master.h"

bool imguiApp::InputFloat2FromVec2(const char* label, DirectX::XMFLOAT2* Vec, const char* format, ImGuiInputTextFlags flags)
{
	return ImGui::InputFloat2(label, (float*)(&Vec->x), format, flags);
}

bool imguiApp::SliderFloat2FromVec2(const char* label, DirectX::XMFLOAT2* Vec, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::SliderFloat2(label, (float*)(&Vec->x), v_min, v_max, format, flags);
}

bool imguiApp::InputFloat3FromVec3(const char* label, DirectX::XMFLOAT3* Vec, const char* format, ImGuiInputTextFlags flags)
{
	return ImGui::InputFloat3(label, (float*)(&Vec->x), format, flags);
}

bool imguiApp::SliderFloat3FromVec3(const char* label, DirectX::XMFLOAT3* Vec, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
	return ImGui::SliderFloat3(label, (float*)(&Vec->x), v_min, v_max, format, flags);
}

imguiApp::imguiApp()
{
	// SRV_CBV_UAV のディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	HRESULT result = D3D12App::Instance()->GetDev()->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(heap.GetAddressOf()));

	if (heap == nullptr)assert(0);

	if (ImGui::CreateContext() == nullptr)assert(0);

	bool blnResult = ImGui_ImplWin32_Init(WinApp::Instance()->GetHWND());
	if (!blnResult)assert(0);

	blnResult = ImGui_ImplDX12_Init(
		D3D12App::Instance()->GetDev(),
		2,
		DXGI_FORMAT_R8G8B8A8_UNORM,	//書き込み先のRTVのフォーマット
		heap.Get(),
		heap.Get()->GetCPUDescriptorHandleForHeapStart(),	//CPUハンドル
		heap.Get()->GetGPUDescriptorHandleForHeapStart());	//GPUハンドル
}

imguiApp::~imguiApp()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void imguiApp::CommandExcute()
{
	if (Master::Instance()->DebugMode())
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (!commandlist.empty())
		{
			/*for (auto command : commandlist)
			{
				command();
			}*/
			commandlist.clear();
		}

		ImGui::Render();
		D3D12App::Instance()->GetCmdList()->SetDescriptorHeaps(1, heap.GetAddressOf());

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), D3D12App::Instance()->GetCmdList());
	}
	else
	{
		if (!commandlist.empty())commandlist.clear();
	}
}