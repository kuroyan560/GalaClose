#pragma once
#include<wrl.h>
#include<DirectXMath.h>
using namespace DirectX;

#include<string>
#include<d3d12.h>
#include<vector>

#include <unordered_map>
#include"Material.h"

class Mesh
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
// 頂点データ構造体（テクスチャあり）
	struct Vertex
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

private:
	// 名前
	std::string name;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// 頂点データ配列
	std::vector<Mesh::Vertex> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// 頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	// マテリアル
	Material* material = nullptr;

public:
	//頂点データの追加
	void AddVertex(const Mesh::Vertex& vertex);

	//頂点インデックスの追加
	void AddIndex(unsigned short index);

	//エッジ平滑化データの追加
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	//平滑化された頂点法線の計算
	void CalculateSmoothedVertexNormals();

	//バッファの生成
	void CreateBuffers();

	void VertMapping();

	//ゲッタ
	inline size_t GetVertexCount() { return vertices.size(); }
	const std::string& GetName() { return name; }
	Material* GetMaterial() { return material; }
	inline const std::vector<Mesh::Vertex>& GetVertices() { return vertices; }
	inline const std::vector<unsigned short>& GetIndices() { return indices; }

	//セッタ
	void SetName(const std::string& Name) { name = Name; }
	void SetMaterial(Material* Material) { this->material = Material; }

	void Draw(int ConstParamIndex, int SRVParamIndex);
};
