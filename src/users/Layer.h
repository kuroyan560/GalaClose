#pragma once
#include<vector>
#include<wrl.h>
#include<d3d12.h>
#include<DirectXMath.h>
#include<string>

class Layer
{
public:
	static const std::string PIPELINE_KEY_MESH;
	static const std::string PIPELINE_KEY_LINE;
	static void CreatePipeline();

private:
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos = { 0,0,0 };
	};
	const enum MESH_TYPE
	{
		BACK_FACE, NEAR_FACE,
		INNER_CIRC_FACE, OUTER_CIRC_FACE,
		MESH_TYPE_NUM
	};
	const enum OUTLINE_TYPE
	{
		OUTLINE_SPLIT_LINE,
		OUTLINE_OUTER_CIRC,
		OUTLINE_TYPE_NUM
	};

	//頂点データ
	std::vector<Vertex> vertices;
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	//面のインデックスデータ
	std::vector<unsigned short>meshIndices;
	ComPtr<ID3D12Resource> meshIndexBuff = nullptr;
	D3D12_INDEX_BUFFER_VIEW meshIbView = {};
	//メッシュ単位のインデックスの数
	int meshIndexCount[MESH_TYPE_NUM] = { 0 };

	//アウトラインエッジ用インデックスデータ
	std::vector<unsigned short>outLineIndices;;
	ComPtr<ID3D12Resource> outLineIndexBuff = nullptr;
	D3D12_INDEX_BUFFER_VIEW outLineIbView = {};
	int outLineIndexCount[OUTLINE_TYPE_NUM] = { 0 };

	//色定数バッファ
	struct Const
	{
		unsigned int color = 0;	//０か１
		float pushedRate = 0.0f;
	};
	unsigned int color = 0;
	ComPtr<ID3D12Resource> constBuff = nullptr;

	int layerNum; //層番号
	const float innerRadius;
	const float outerRadius;

	float explosionLayerRateOffset = 0.0f;

public:
	Layer(const float& StartAngle, const float& EndAngle, const int& LayerNum, const unsigned int& Color, const float& PushedRate = 0.0f);
	void Update(const float& PushedRate);
	void Draw();

	const unsigned int& GetLayerNum() { return layerNum; }
	const unsigned int& GetColor() { return color; }

	void SetExplosionOffset();
};
