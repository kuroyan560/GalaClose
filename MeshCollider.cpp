#include "MeshCollider.h"
#include"Collision.h"

using namespace DirectX;

void MeshCollider::ConstructTriangles(OBJModel* model)
{
	//三角形リストをクリア
	trianglesData.clear();
	//モデルの持つメッシュリストを取得
	const std::vector<Mesh*>& meshes = model->GetMeshes();
	//現在のメッシュの開始三角形番号を淹れておく変数（０で初期化）
	int start = 0;
	//全メッシュについて順に処理する
	std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	trianglesData.emplace_back();
	trianglesData.back().attribute = 0;

	XMFLOAT3 tmp = { 0,0,0 };
	int vertexTotal = 0;

	for (int i = 0; i < meshes.size(); ++i)
	{
		//インデックスは、三角形の数×３個あるので、そこからメッシュ内の三角形の数を逆算する
		size_t triangleNum = meshes[i]->GetIndices().size() / 3;
		//現在のメッシュの三角形の数だけ、三角形リストにスペースを追加する
		trianglesData.back().triangles.resize(trianglesData.back().triangles.size() + triangleNum);
		//全三角形について順に処理する
		for (int j = 0; j < triangleNum; j++)
		{
			//今から計算する三角形の参照
			Triangle& tri = trianglesData.back().triangles[start + j];
			int idx0 = meshes[i]->GetIndices()[j * 3];
			int idx1 = meshes[i]->GetIndices()[j * 3 + 1];
			int idx2 = meshes[i]->GetIndices()[j * 3 + 2];
			//三角形の３頂点の座標を代入
			tri.p0 = { meshes[i]->GetVertices()[idx0].pos.x,meshes[i]->GetVertices()[idx0].pos.y,meshes[i]->GetVertices()[idx0].pos.z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, meshes[i]->GetVertices()[idx0].pos);
			tri.p1 = { meshes[i]->GetVertices()[idx1].pos.x,meshes[i]->GetVertices()[idx1].pos.y,meshes[i]->GetVertices()[idx1].pos.z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, meshes[i]->GetVertices()[idx1].pos);
			tri.p2 = { meshes[i]->GetVertices()[idx2].pos.x,meshes[i]->GetVertices()[idx2].pos.y,meshes[i]->GetVertices()[idx2].pos.z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, meshes[i]->GetVertices()[idx2].pos);
			//３頂点から法線を計算
			tri.ComputeNormal();
			vertexTotal += 3;
		}
		//次のメッシュは、今までの三角形番号の次から使う
		start += (int)triangleNum;
	}
	tmp = MyFunc::DivXMFLOAT3(tmp, vertexTotal);
	trianglesData.back().center = tmp;
	trianglesData.back().recentCenter = tmp;
}

void MeshCollider::ConstructTriangles(Model_HSM* Model)
{
	//三角形リストをクリア
	trianglesData.clear();
	int idxOffset = 0;
	//現在のメッシュの開始三角形番号を淹れておく変数（０で初期化）
	int start = 0;

	trianglesData.emplace_back();
	trianglesData.back().attribute = 0;

	XMFLOAT3 tmp = { 0,0,0 };

	//全メッシュについて順に処理する
	for (int i = 0; i < Model->GetMeshes().size(); ++i)
	{
		//インデックスは、三角形の数×３個あるので、そこからメッシュ内の三角形の数を逆算する
		size_t triangleNum = Model->GetMeshes()[i].vertexNum / 3;
		//現在のメッシュの三角形の数だけ、三角形リストにスペースを追加する
		trianglesData.back().triangles.resize(trianglesData.back().triangles.size() + triangleNum);
		//全三角形について順に処理する
		for (int k = 0; k < triangleNum; k++)
		{
			//今から計算する三角形の参照
			Triangle& tri = trianglesData.back().triangles[start + k];
			int idx0 = Model->GetIndex(idxOffset + k * 3);
			int idx1 = Model->GetIndex(idxOffset + k * 3 + 1);
			int idx2 = Model->GetIndex(idxOffset + k * 3 + 2);
			//三角形の３頂点の座標を代入
			tri.p0 = { Model->GetVertexPos(idx0).x,Model->GetVertexPos(idx0).y,Model->GetVertexPos(idx0).z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx0));
			tri.p2 = { Model->GetVertexPos(idx1).x,Model->GetVertexPos(idx1).y,Model->GetVertexPos(idx1).z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx1));
			tri.p1 = { Model->GetVertexPos(idx2).x,Model->GetVertexPos(idx2).y,Model->GetVertexPos(idx2).z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx2));
			//３頂点から法線を計算
			tri.ComputeNormal();
		}
		//次のメッシュは、今までの三角形番号の次から使う
		start += (int)triangleNum;
		idxOffset += Model->GetMeshes()[i].vertexNum;
	}

	tmp = MyFunc::DivXMFLOAT3(tmp, idxOffset);
	trianglesData.back().center = tmp;
	trianglesData.back().recentCenter = tmp;
}

void MeshCollider::ConstructTriangles(Model_HSM* Model, std::string MeshName, int Attribute)
{
	//三角形リストをクリア
	trianglesData.clear();

	//頂点インデックスオフセット
	unsigned int idxOffset = 0;

	//全メッシュについて順に処理する
	for (int i = 0; i < Model->GetMeshes().size(); ++i)
	{
		//メッシュリスト
		if (Model->GetMeshes()[i].name == MeshName)
		{
			trianglesData.emplace_back();
			trianglesData.back().attribute = Attribute;

			XMFLOAT3 tmp = { 0,0,0 };

			//インデックスは、三角形の数×３個あるので、そこからメッシュ内の三角形の数を逆算する
			size_t triangleNum = Model->GetMeshes()[i].vertexNum / 3;
			//現在のメッシュの三角形の数だけ、三角形リストにスペースを追加する
			trianglesData.back().triangles.resize(triangleNum);
			//全三角形について順に処理する
			for (int k = 0; k < triangleNum; k++)
			{
				//今から計算する三角形の参照
				Triangle& tri = trianglesData.back().triangles[k];
				int idx0 = Model->GetIndex(idxOffset + k * 3);
				int idx1 = Model->GetIndex(idxOffset + k * 3 + 1);
				int idx2 = Model->GetIndex(idxOffset + k * 3 + 2);
				//三角形の３頂点の座標を代入
				tri.p0 = { Model->GetVertexPos(idx0).x,Model->GetVertexPos(idx0).y,Model->GetVertexPos(idx0).z,1 };
				tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx0));
				tri.p2 = { Model->GetVertexPos(idx1).x,Model->GetVertexPos(idx1).y,Model->GetVertexPos(idx1).z,1 };
				tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx1));
				tri.p1 = { Model->GetVertexPos(idx2).x,Model->GetVertexPos(idx2).y,Model->GetVertexPos(idx2).z,1 };
				tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx2));
				//３頂点から法線を計算
				tri.ComputeNormal();
			}

			tmp = MyFunc::DivXMFLOAT3(tmp, Model->GetMeshes()[i].vertexNum);
			trianglesData.back().center = tmp;
			trianglesData.back().recentCenter = tmp;
			break;
		}
		idxOffset += Model->GetMeshes()[i].vertexNum;
	}
}

void MeshCollider::ForciblyUpdate()
{
	//ワールド行列の逆行列を計算
	invMatWorld = XMMatrixInverse(nullptr, object->GetTransform().mat);

	for (auto itr = trianglesData.begin(); itr != trianglesData.end(); ++itr)
	{
		XMVECTOR vec = XMLoadFloat3(&itr->center);
		vec = XMVector3Transform(vec, object->GetTransform().mat);
		XMStoreFloat3(&itr->recentCenter, vec);
	}
}

MeshCollider::MeshCollider(OBJModel* Model, int ID)
	:BaseCollider(COLLISION_SHAPE_MESH, ID)
{
	ConstructTriangles(Model);
}

MeshCollider::MeshCollider(Model_HSM* Model, int ID)
	: BaseCollider(COLLISION_SHAPE_MESH, ID)
{
	ConstructTriangles(Model);
}

MeshCollider::MeshCollider(int ID)
	: BaseCollider(COLLISION_SHAPE_MESH, ID)
{
	
}

void MeshCollider::AddMeshInfo(Model_HSM* Model, std::string MeshName, int Attribute)
{
	ConstructTriangles(Model, MeshName, Attribute);
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
{
	//オブジェクトのローカル座標系での球を得る（半径はXスケールを参照）
	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];


	for (int i = 0; i < trianglesData.size(); ++i)
	{
		//ローカル座標系で交差をチェック
		std::vector<Triangle>::const_iterator it = trianglesData[i].triangles.cbegin();

		for (; it != trianglesData[i].triangles.cend(); ++it)
		{
			const Triangle& triangle = *it;

			//球と三角形の当たり判定
			if (Collision::CheckSphere2Triangle(localSphere, triangle, inter))
			{
				if (inter)
				{
					const XMMATRIX& matWorld = object->GetTransform().mat;
					//ワールド座標系での交点を得る
					*inter = XMVector3Transform(*inter, matWorld);
				}
				return true;
			}
		}
	}
	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, DirectX::XMVECTOR* inter, int* meshIndex, int* meshAttribute)
{
	//オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = XMVector3Transform(ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(ray.dir, invMatWorld);

	for (int i = 0; i < trianglesData.size(); ++i)
	{
		//ローカル座標系で交差をチェック
		std::vector<Triangle>::const_iterator it = trianglesData[i].triangles.cbegin();

		for (; it != trianglesData[i].triangles.cend(); ++it)
		{
			const Triangle& triangle = *it;

			XMVECTOR tmpInter;
			//レイと三角形の当たり判定
			if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tmpInter))
			{
				const XMMATRIX& matWorld = object->GetTransform().mat;
				//ワールド座標系での交点を得る
				tmpInter = XMVector3Transform(tmpInter, matWorld);

				if (distance)
				{
					//交点とレイ始点の距離を計算
					XMVECTOR sub = tmpInter - ray.start;
					*distance = XMVector3Dot(sub, ray.dir).m128_f32[0];
				}
				if (inter)
				{
					*inter = tmpInter;
				}
				if (meshIndex)
				{
					*meshIndex = i;
				}
				if (meshAttribute)
				{
					*meshAttribute = trianglesData[i].attribute;
				}
				return true;
			}
		}
	}
	return false;
}

XMFLOAT3* MeshCollider::GetMeshColCenter(int MeshDataIndex)
{
	return &trianglesData[MeshDataIndex].recentCenter;
}
