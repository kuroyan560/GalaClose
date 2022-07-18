#pragma once

#include"BaseCollider.h"
#include"CollisionPrimitive.h"

#include<DirectXMath.h>

#include"OBJModel.h"

class MeshCollider : public BaseCollider
{
private:
	struct MeshColliderData
	{
		XMFLOAT3 center;	//メッシュの中心
		XMFLOAT3 recentCenter;
		std::vector<Triangle> triangles;
		int attribute = -1;
	};
	std::vector<MeshColliderData>trianglesData;
	//ワールド行列の逆行列
	DirectX::XMMATRIX invMatWorld;

	void ConstructTriangles(OBJModel* model);
	void ConstructTriangles(Model_HSM* Model);
	void ConstructTriangles(Model_HSM* Model, std::string MeshName, int Attribute);

	void ForciblyUpdate()override;

public:
	MeshCollider(OBJModel* Model,int ID = -1);
	MeshCollider(Model_HSM* Model,int ID = -1);
	MeshCollider(int ID = -1);
	void AddMeshInfo(Model_HSM* Model, std::string MeshName, int Attribute);

	bool CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter = nullptr);

	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr, int* meshIndex = nullptr,int* meshAttribute = nullptr);

	void SetBroadPhase(BaseCollider* BroadPhase) 
	{ 
		broadPhase = BroadPhase; 
	}
	BaseCollider* GetBroad() 
	{ 
		return broadPhase; 
	}
	XMFLOAT3* GetMeshColCenter(int MeshDataIndex);
};

