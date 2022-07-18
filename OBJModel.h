#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include"Mesh.h"
#include"Object3D.h"
class OBJModel : public Object3D
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	static const string MODEL_DIRECTORY_PATH;
	static const string PIPELINE_KEY;
public:
	static void CreatePipeline();

protected:
	// ���O
	std::string name;
	// ���b�V���R���e�i
	std::vector<Mesh*> meshes;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;

	void LoadModel(const std::string& ModelName, bool Smoothing);
	void LoadMaterial(const std::string& DirectoryPath, const std::string& Filename);

	void AddMaterial(Material* material)
	{
		// �R���e�i�ɓo�^
		materials.emplace(material->name, material);
	}

public:
	OBJModel(const std::string& ModelName, bool Smoothing = false,
		XMFLOAT3 Pos = { 0,0,0 }, XMFLOAT3 Scale = { 1,1,1 }, XMFLOAT3 Rotation = { 0,0,0 }, int CollisionRadius = 3);
	virtual ~OBJModel();

	virtual void UniqueInit()override {};
	virtual void UniqueUpdate()override {};
	virtual void UniqueFinalize()override {};
	void Draw()override;

	void OnBillBoard();
	void OnBillBoardY();

	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }
	const string& GetName() { return name; }
};

