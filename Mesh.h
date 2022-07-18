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

public: // �T�u�N���X
// ���_�f�[�^�\���́i�e�N�X�`������j
	struct Vertex
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

private:
	// ���O
	std::string name;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// ���_�f�[�^�z��
	std::vector<Mesh::Vertex> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// ���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	// �}�e���A��
	Material* material = nullptr;

public:
	//���_�f�[�^�̒ǉ�
	void AddVertex(const Mesh::Vertex& vertex);

	//���_�C���f�b�N�X�̒ǉ�
	void AddIndex(unsigned short index);

	//�G�b�W�������f�[�^�̒ǉ�
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	//���������ꂽ���_�@���̌v�Z
	void CalculateSmoothedVertexNormals();

	//�o�b�t�@�̐���
	void CreateBuffers();

	void VertMapping();

	//�Q�b�^
	inline size_t GetVertexCount() { return vertices.size(); }
	const std::string& GetName() { return name; }
	Material* GetMaterial() { return material; }
	inline const std::vector<Mesh::Vertex>& GetVertices() { return vertices; }
	inline const std::vector<unsigned short>& GetIndices() { return indices; }

	//�Z�b�^
	void SetName(const std::string& Name) { name = Name; }
	void SetMaterial(Material* Material) { this->material = Material; }

	void Draw(int ConstParamIndex, int SRVParamIndex);
};
