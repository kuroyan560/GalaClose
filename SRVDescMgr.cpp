#include "SRVDescMgr.h"
using namespace DirectX;

#include"D3D12App.h"
#include"WinApp.h"

#include<algorithm>

#include"MyFunc.h"

SRVDescMgr::SRVDescMgr()
	:heap(CBV_SRV_UAV, D3D12App::Instance()->GetDev())
{
	loadLambdaTable["sph"]
		= loadLambdaTable["spa"]
		= loadLambdaTable["bmp"]
		= loadLambdaTable["png"]
		= loadLambdaTable["jpg"]
		= [](const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)->HRESULT
	{
		return LoadFromWICFile(Path.c_str(), WIC_FLAGS_NONE, Meta, Img);
	};
	loadLambdaTable["tga"]
		= [](const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)->HRESULT
	{
		return LoadFromTGAFile(Path.c_str(), Meta, Img);
	};
	loadLambdaTable["dds"]
		= [](const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)->HRESULT
	{
		return LoadFromDDSFile(Path.c_str(), DDS_FLAGS_NONE, Meta, Img);
	};
}

int SRVDescMgr::CreateSRV(const string& FilePass)
{
	//既にロードしたか検索
	if (!texInfo.empty())
	{
		for (auto itr = texInfo.begin(); itr != texInfo.end(); itr++)
		{
			if (itr->filePass == FilePass)
			{
				return itr->texHandle;
			}
		}
	}

	HRESULT result;
	/*
		WICテクスチャのロード
		metadataには画像サイズなど、テクスチャとしての各種情報が入り、
		imgポインタにビットマップ形式での画像データそのもののアドレスが入る
	*/
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	auto wtexpath = MyFunc::GetWideStrFromStr(FilePass);
	auto ext = MyFunc::GetExtension(FilePass);

	result = loadLambdaTable[ext](
		wtexpath,
		&metadata,
		scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0);	//生データ抽出

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"画像のデータ抽出に失敗");
		return -1;
	}

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,	//RGBAフォーマット
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//テクスチャ用リソースバッファの生成
	texInfo.push_back(Texture(FilePass, heap.GetDescNum(), texresDesc));
	CD3DX12_HEAP_PROPERTIES heapPropForTex(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&heapPropForTex,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texInfo.back().buff));

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"画像リソースバッファ生成に失敗");
		return -1;
	}

	//テクスチャバッファにデータ転送
	result = texInfo.back().buff->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		img->pixels,	//元データアドレス
		(UINT)img->rowPitch,	//1ラインサイズ
		(UINT)img->slicePitch	//１枚サイズ
	);

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"画像のリソースバッファ転送に失敗");
		return -1;
	}

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープにシェーダリソースビュー作成
	D3D12App::Instance()->GetDev()->CreateShaderResourceView(
		texInfo.back().buff.Get(),	//ビューと関連付けるバッファ
		&srvDesc,				//テクスチャ設定情報
		heap.GetCpuHandle());

	heap.AddDescNum();

	return heap.GetDescNum() - 1;
}

int SRVDescMgr::CreateSRV(int R, int G, int B)
{
	//既にロードしたか検索
	if (!colorTex.empty())
	{
		for (auto itr = colorTex.begin(); itr != colorTex.end(); itr++)
		{
			if (itr->r == R && itr->g == G && itr->b == B)
			{
				return itr->texHandle;
			}
		}
	}

	HRESULT result;

	const int texWidth = 256;
	const int texDataCount = texWidth * texWidth;
	//テクスチャデータ配列
	XMFLOAT4* texturedata = new XMFLOAT4[texDataCount];

	//全ピクセルの色を初期化
	for (int i = 0; i < texDataCount; i++)
	{
		texturedata[i].x = (float)R / 255.0f;	//R
		texturedata[i].y = (float)G / 255.0f;	//G
		texturedata[i].z = (float)B / 255.0f;	//B
		texturedata[i].w = 1.0f;	//A
	}

	D3D12_HEAP_PROPERTIES texHeapProp{};	//テクスチャヒープ設定
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//テクスチャリソース設定
	D3D12_RESOURCE_DESC texresDesc{};	//リソース設定
	texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2Dテクスチャ用
	texresDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//RGBAフォーマット
	texresDesc.Width = texWidth;
	texresDesc.Height = texWidth;
	texresDesc.DepthOrArraySize = 1;
	texresDesc.MipLevels = 1;
	texresDesc.SampleDesc.Count = 1;

	//テクスチャ用リソースバッファの生成
	colorTex.push_back(ColorTexture(heap.GetDescNum(), R, G, B));

	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&colorTex.back().buff));

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"単色塗りつぶしリソースバッファの生成に失敗");
		return -1;
	}

	//テクスチャバッファにデータ転送
	result = colorTex.back().buff->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		texturedata,	//元データアドレス
		sizeof(XMFLOAT4) * texWidth,	//1ラインサイズ
		sizeof(XMFLOAT4) * texDataCount	//１枚サイズ
	);

	delete[] texturedata;

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"単色塗りつぶしリソースバッファの転送に失敗");
		return -1;
	}

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープにシェーダリソースビュー作成
	D3D12App::Instance()->GetDev()->CreateShaderResourceView(
		colorTex.back().buff.Get(),	//ビューと関連付けるバッファ
		&srvDesc,				//テクスチャ設定情報
		heap.GetCpuHandle());

	heap.AddDescNum();

	return heap.GetDescNum() - 1;
}

int SRVDescMgr::CreateSRV(ID3D12Resource* Resource, DXGI_FORMAT ColorFormat)
{
	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = ColorFormat;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープにシェーダリソースビュー作成
	D3D12App::Instance()->GetDev()->CreateShaderResourceView(
		Resource,	//ビューと関連付けるバッファ
		&srvDesc,				//テクスチャ設定情報
		heap.GetCpuHandle());

	heap.AddDescNum();

	return heap.GetDescNum() - 1;
}

void SRVDescMgr::SetSRV(const int& SRVHandle, const int& RootParamIndex)
{
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootDescriptorTable(RootParamIndex, heap.GetGpuHandle(SRVHandle));
}

const CD3DX12_RESOURCE_DESC& SRVDescMgr::GetTexDesc(const int& SRVHandle)
{
	// TODO: return ステートメントをここに挿入します
	auto result = std::find_if(texInfo.begin(), texInfo.end(), [SRVHandle](Texture Tex) { return Tex.texHandle == SRVHandle; });
	return result->texResDesc;
}
