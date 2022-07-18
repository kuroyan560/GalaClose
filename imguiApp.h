#pragma once
#include<wrl.h>
#include"Singleton.h"
#include<d3d12.h>
#include<string>
#include<vector>
#include<DirectXMath.h>
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"

using namespace std;
typedef void (*func)();

class imguiApp : public Singleton<imguiApp>
{
private:
#pragma region static
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	friend Singleton<imguiApp>;
	imguiApp();
#pragma endregion

public:
	static bool InputFloat2FromVec2(const char* label, DirectX::XMFLOAT2* Vec, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
	static bool SliderFloat2FromVec2(const char* label, DirectX::XMFLOAT2* Vec, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	static bool InputFloat3FromVec3(const char* label, DirectX::XMFLOAT3* Vec, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
	static bool SliderFloat3FromVec3(const char* label, DirectX::XMFLOAT3* Vec, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

private:
	//imguiコマンドリスト（自作）
	vector<void(*)()>commandlist;

	ComPtr<ID3D12DescriptorHeap>heap;

public:
	~imguiApp();
	void CommandExcute();

	//imguiの描画関数をリストに追加
	void AddCommand(void(*Func)()) { commandlist.push_back(Func); }
};

