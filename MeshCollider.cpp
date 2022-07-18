#include "MeshCollider.h"
#include"Collision.h"

using namespace DirectX;

void MeshCollider::ConstructTriangles(OBJModel* model)
{
	//�O�p�`���X�g���N���A
	trianglesData.clear();
	//���f���̎����b�V�����X�g���擾
	const std::vector<Mesh*>& meshes = model->GetMeshes();
	//���݂̃��b�V���̊J�n�O�p�`�ԍ��🹂�Ă����ϐ��i�O�ŏ������j
	int start = 0;
	//�S���b�V���ɂ��ď��ɏ�������
	std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	trianglesData.emplace_back();
	trianglesData.back().attribute = 0;

	XMFLOAT3 tmp = { 0,0,0 };
	int vertexTotal = 0;

	for (int i = 0; i < meshes.size(); ++i)
	{
		//�C���f�b�N�X�́A�O�p�`�̐��~�R����̂ŁA�������烁�b�V�����̎O�p�`�̐����t�Z����
		size_t triangleNum = meshes[i]->GetIndices().size() / 3;
		//���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
		trianglesData.back().triangles.resize(trianglesData.back().triangles.size() + triangleNum);
		//�S�O�p�`�ɂ��ď��ɏ�������
		for (int j = 0; j < triangleNum; j++)
		{
			//������v�Z����O�p�`�̎Q��
			Triangle& tri = trianglesData.back().triangles[start + j];
			int idx0 = meshes[i]->GetIndices()[j * 3];
			int idx1 = meshes[i]->GetIndices()[j * 3 + 1];
			int idx2 = meshes[i]->GetIndices()[j * 3 + 2];
			//�O�p�`�̂R���_�̍��W����
			tri.p0 = { meshes[i]->GetVertices()[idx0].pos.x,meshes[i]->GetVertices()[idx0].pos.y,meshes[i]->GetVertices()[idx0].pos.z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, meshes[i]->GetVertices()[idx0].pos);
			tri.p1 = { meshes[i]->GetVertices()[idx1].pos.x,meshes[i]->GetVertices()[idx1].pos.y,meshes[i]->GetVertices()[idx1].pos.z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, meshes[i]->GetVertices()[idx1].pos);
			tri.p2 = { meshes[i]->GetVertices()[idx2].pos.x,meshes[i]->GetVertices()[idx2].pos.y,meshes[i]->GetVertices()[idx2].pos.z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, meshes[i]->GetVertices()[idx2].pos);
			//�R���_����@�����v�Z
			tri.ComputeNormal();
			vertexTotal += 3;
		}
		//���̃��b�V���́A���܂ł̎O�p�`�ԍ��̎�����g��
		start += (int)triangleNum;
	}
	tmp = MyFunc::DivXMFLOAT3(tmp, vertexTotal);
	trianglesData.back().center = tmp;
	trianglesData.back().recentCenter = tmp;
}

void MeshCollider::ConstructTriangles(Model_HSM* Model)
{
	//�O�p�`���X�g���N���A
	trianglesData.clear();
	int idxOffset = 0;
	//���݂̃��b�V���̊J�n�O�p�`�ԍ��🹂�Ă����ϐ��i�O�ŏ������j
	int start = 0;

	trianglesData.emplace_back();
	trianglesData.back().attribute = 0;

	XMFLOAT3 tmp = { 0,0,0 };

	//�S���b�V���ɂ��ď��ɏ�������
	for (int i = 0; i < Model->GetMeshes().size(); ++i)
	{
		//�C���f�b�N�X�́A�O�p�`�̐��~�R����̂ŁA�������烁�b�V�����̎O�p�`�̐����t�Z����
		size_t triangleNum = Model->GetMeshes()[i].vertexNum / 3;
		//���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
		trianglesData.back().triangles.resize(trianglesData.back().triangles.size() + triangleNum);
		//�S�O�p�`�ɂ��ď��ɏ�������
		for (int k = 0; k < triangleNum; k++)
		{
			//������v�Z����O�p�`�̎Q��
			Triangle& tri = trianglesData.back().triangles[start + k];
			int idx0 = Model->GetIndex(idxOffset + k * 3);
			int idx1 = Model->GetIndex(idxOffset + k * 3 + 1);
			int idx2 = Model->GetIndex(idxOffset + k * 3 + 2);
			//�O�p�`�̂R���_�̍��W����
			tri.p0 = { Model->GetVertexPos(idx0).x,Model->GetVertexPos(idx0).y,Model->GetVertexPos(idx0).z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx0));
			tri.p2 = { Model->GetVertexPos(idx1).x,Model->GetVertexPos(idx1).y,Model->GetVertexPos(idx1).z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx1));
			tri.p1 = { Model->GetVertexPos(idx2).x,Model->GetVertexPos(idx2).y,Model->GetVertexPos(idx2).z,1 };
			tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx2));
			//�R���_����@�����v�Z
			tri.ComputeNormal();
		}
		//���̃��b�V���́A���܂ł̎O�p�`�ԍ��̎�����g��
		start += (int)triangleNum;
		idxOffset += Model->GetMeshes()[i].vertexNum;
	}

	tmp = MyFunc::DivXMFLOAT3(tmp, idxOffset);
	trianglesData.back().center = tmp;
	trianglesData.back().recentCenter = tmp;
}

void MeshCollider::ConstructTriangles(Model_HSM* Model, std::string MeshName, int Attribute)
{
	//�O�p�`���X�g���N���A
	trianglesData.clear();

	//���_�C���f�b�N�X�I�t�Z�b�g
	unsigned int idxOffset = 0;

	//�S���b�V���ɂ��ď��ɏ�������
	for (int i = 0; i < Model->GetMeshes().size(); ++i)
	{
		//���b�V�����X�g
		if (Model->GetMeshes()[i].name == MeshName)
		{
			trianglesData.emplace_back();
			trianglesData.back().attribute = Attribute;

			XMFLOAT3 tmp = { 0,0,0 };

			//�C���f�b�N�X�́A�O�p�`�̐��~�R����̂ŁA�������烁�b�V�����̎O�p�`�̐����t�Z����
			size_t triangleNum = Model->GetMeshes()[i].vertexNum / 3;
			//���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
			trianglesData.back().triangles.resize(triangleNum);
			//�S�O�p�`�ɂ��ď��ɏ�������
			for (int k = 0; k < triangleNum; k++)
			{
				//������v�Z����O�p�`�̎Q��
				Triangle& tri = trianglesData.back().triangles[k];
				int idx0 = Model->GetIndex(idxOffset + k * 3);
				int idx1 = Model->GetIndex(idxOffset + k * 3 + 1);
				int idx2 = Model->GetIndex(idxOffset + k * 3 + 2);
				//�O�p�`�̂R���_�̍��W����
				tri.p0 = { Model->GetVertexPos(idx0).x,Model->GetVertexPos(idx0).y,Model->GetVertexPos(idx0).z,1 };
				tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx0));
				tri.p2 = { Model->GetVertexPos(idx1).x,Model->GetVertexPos(idx1).y,Model->GetVertexPos(idx1).z,1 };
				tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx1));
				tri.p1 = { Model->GetVertexPos(idx2).x,Model->GetVertexPos(idx2).y,Model->GetVertexPos(idx2).z,1 };
				tmp = MyFunc::AddXMFLOAT3(tmp, Model->GetVertexPos(idx2));
				//�R���_����@�����v�Z
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
	//���[���h�s��̋t�s����v�Z
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
	//�I�u�W�F�N�g�̃��[�J�����W�n�ł̋��𓾂�i���a��X�X�P�[�����Q�Ɓj
	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];


	for (int i = 0; i < trianglesData.size(); ++i)
	{
		//���[�J�����W�n�Ō������`�F�b�N
		std::vector<Triangle>::const_iterator it = trianglesData[i].triangles.cbegin();

		for (; it != trianglesData[i].triangles.cend(); ++it)
		{
			const Triangle& triangle = *it;

			//���ƎO�p�`�̓����蔻��
			if (Collision::CheckSphere2Triangle(localSphere, triangle, inter))
			{
				if (inter)
				{
					const XMMATRIX& matWorld = object->GetTransform().mat;
					//���[���h���W�n�ł̌�_�𓾂�
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
	//�I�u�W�F�N�g�̃��[�J�����W�n�ł̃��C�𓾂�
	Ray localRay;
	localRay.start = XMVector3Transform(ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(ray.dir, invMatWorld);

	for (int i = 0; i < trianglesData.size(); ++i)
	{
		//���[�J�����W�n�Ō������`�F�b�N
		std::vector<Triangle>::const_iterator it = trianglesData[i].triangles.cbegin();

		for (; it != trianglesData[i].triangles.cend(); ++it)
		{
			const Triangle& triangle = *it;

			XMVECTOR tmpInter;
			//���C�ƎO�p�`�̓����蔻��
			if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tmpInter))
			{
				const XMMATRIX& matWorld = object->GetTransform().mat;
				//���[���h���W�n�ł̌�_�𓾂�
				tmpInter = XMVector3Transform(tmpInter, matWorld);

				if (distance)
				{
					//��_�ƃ��C�n�_�̋������v�Z
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
