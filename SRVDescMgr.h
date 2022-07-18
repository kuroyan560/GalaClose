#pragma once
#include<wrl.h>
#include<string.h>
using namespace std;

#include"Singleton.h"
#include"DescHeap.h"

#include<map>
#include<functional>
#include<DirectXTex.h>
using namespace DirectX;

class SRVDescMgr :public Singleton<SRVDescMgr>
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	SRVDescMgr();
	friend Singleton<SRVDescMgr>;

#pragma region テクスチャ情報保存用構造体
	//読み込んだテクスチャの情報構造体
	struct Texture
	{
		string filePass;			//ファイルパス
		int texHandle;							//割り当てられたハンドル番号
		ComPtr<ID3D12Resource>buff;			//テクスチャリソースのバッファ
		CD3DX12_RESOURCE_DESC texResDesc;	//テクスチャの情報（横幅、高さなど）
		Texture(const string& FilePass, int Handle, CD3DX12_RESOURCE_DESC TexResDesc)
			:filePass(FilePass), texHandle(Handle), texResDesc(TexResDesc) {};
	};

	//単色塗りつぶし用テクスチャ
	struct ColorTexture
	{
		int r;
		int g;
		int b;

		int texHandle;
		ComPtr<ID3D12Resource>buff;

		ColorTexture(int Handle, int R, int G, int B)
			:texHandle(Handle), r(R), g(G), b(B) {};
	};
#pragma endregion

	DescHeap heap;

	//読み込んだテクスチャの情報管理
	std::vector<Texture>texInfo;
	//作った単色テクスチャの情報管理
	std::vector<ColorTexture>colorTex;

	//ロード用のラムダ式のための型
	using LoadLambda_t = std::function<HRESULT(const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)>;
	std::map<std::string, LoadLambda_t>loadLambdaTable;

public:
	int CreateSRV(const string& FilePass);
	int CreateSRV(int R, int G, int B);
	//GPUリソースから作成
	int CreateSRV(ID3D12Resource* Resource, DXGI_FORMAT ColorFormat);

	void SetSRV(const int& SRVHandle, const int& RootParamIndex);

	const CD3DX12_RESOURCE_DESC& GetTexDesc(const int& SRVHandle);
};