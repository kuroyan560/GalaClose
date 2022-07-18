#pragma once
#include<string>
#include<DirectXMath.h>
using namespace DirectX;

#include<wrl.h>
#include<d3d12.h>

class Material
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct Const
	{
		XMFLOAT3 ambient; // アンビエント係数
		float pad1; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2; // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha;	// アルファ
	};
public:
	static Material* Create();

private:
	Material()
	{
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

public:
	std::string name;	// マテリアル名
	XMFLOAT3 ambient;	// アンビエント影響度
	XMFLOAT3 diffuse;	// ディフューズ影響度
	XMFLOAT3 specular;	// スペキュラー影響度
	float alpha;		// アルファ
	int texHandle = -1;

	void ConstMapping();
	void Set(int ConstParamIndex, int SRVParamIndex);
};