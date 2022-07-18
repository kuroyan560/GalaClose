#include "FbxLoader.h"
#include"WinApp.h"
#include"MyFunc.h"
using namespace MyDef;
#include<vector>
#include"SRVDescMgr.h"
#include"Action.h"

XMMATRIX ConvertToXMMATRIX(FbxMatrix Mat)
{
	XMMATRIX result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.r[i].m128_f32[j] = Mat[i][j];
		}
	}
	return result;
}

string GetFileNameFromPath(string Path)
{
	auto strItr = Path.begin();

	for (auto itr = Path.begin(); itr != Path.end(); itr++)
	{
		if (*itr == '\\')
		{
			*itr = '/';
		}
		if (*itr == '/')
		{
			strItr = itr + 1;
		}
	}
	string result;
	for (auto itr = strItr; itr != Path.end(); itr++)
	{
		result += *itr;
	}

	return result;
}

void FBXLoader::GetMesh(FbxNode* Node, std::vector<FbxMesh*>* Mesh)
{
	int childCount = Node->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* child = Node->GetChild(i);	//�q�m�[�h���擾
		int meshCount = child->GetNodeAttributeCount();
		for (int num = 0; num < meshCount; num++)
		{
			FbxNodeAttribute* info = child->GetNodeAttributeByIndex(num);
			FbxNodeAttribute::EType type = info->GetAttributeType();

			if (type == FbxNodeAttribute::EType::eMesh)
			{
				Mesh->emplace_back((FbxMesh*)info);
			}
		}
		GetMesh(child, Mesh);
	}
}

void FBXLoader::SetBoneAffectToVertex(
	std::map<int, std::forward_list<BoneAffectInfo>>& BoneAffectTable, HSM::Vertex* VertexPtr, int Index)
{
	//�e���f�[�^�\���󂶂�Ȃ�
	if (!BoneAffectTable[Index].empty())
	{
		//�K�p�����{�[���̐�
		int count = 0;

		//�Y���C���f�b�N�X�̉e���f�[�^�ꗗ���Q��
		for (auto itr = BoneAffectTable[Index].begin(); itr != BoneAffectTable[Index].end(); ++itr)
		{
			for (int i = 0; i < 4; ++i)
			{
				//�Ώۂ̒��_�̃{�[���f�[�^�ŋ�ȗ̈�Ƀf�[�^��ۑ�
				if (VertexPtr->boneIndex[i] == -1)	//�{�[�����o�^
				{
					VertexPtr->boneIndex[i] = itr->index;
					VertexPtr->boneWeight[i] = itr->weight;
					break;
				}
			}
			count++;
		}

		//�P�̒��_�ɂ��ő�S�̃{�[����K�p�o���Ȃ����߁A�T�ڈȍ~�̏��͖�������Ă��邱�Ƃ��x��
		if (4 < count)
		{
			WinApp::Instance()->
				CreateMessageWindow("WARNING", "FBXLoader�̒��_�ǂݍ��݂ɂĂT�ȏ�̃{�[�����K�p����܂����B�T�ڈȍ~�͖�������܂�");
		}
	}
}

void FBXLoader::LoadConvertFBXVertex(FbxMesh* Mesh, Model_HSM* Model, std::map<int, std::forward_list<BoneAffectInfo>>& Info)
{
	//���_�o�b�t�@�̎擾
	FbxVector4* vertices = Mesh->GetControlPoints();
	//�C���f�b�N�X�o�b�t�@�̎擾
	int* indices = Mesh->GetPolygonVertices();
	//���_���W�̐��̎擾
	int polygonVertexCount = Mesh->GetPolygonVertexCount();

	for (int i = 0; i < polygonVertexCount; i++)
	{
		Model->vertices.emplace_back();

		//�C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[i];

		float3 pos;
		//���_���W���X�g������W���擾
		pos.x = vertices[index][0];
		pos.y = vertices[index][1];
		pos.z = vertices[index][2];

		Model->vertices.back().pos = pos;

		//�ۑ����Ă������{�[���̑Ή��\���璸�_�ɉe���f�[�^���擾
		SetBoneAffectToVertex(Info, &(Model->vertices.back()), index);
	}
}

void FBXLoader::LoadConvertFBXIndex(std::vector<FbxMesh*>* Meshes, Model_HSM* Model)
{
	int offset = 0;
	for (int meshNum = 0; meshNum < Meshes->size(); meshNum++)
	{
		//�|���S���̐������A�ԂƂ��ĕۑ�����
		for (int i = 0; i < Meshes->at(meshNum)->GetPolygonCount(); i++)
		{
			//����n�i�E����j
			Model->indices.emplace_back(offset + i * 3 + 1);
			Model->indices.emplace_back(offset + i * 3 + 2);
			Model->indices.emplace_back(offset + i * 3);
		}
		offset += Meshes->at(meshNum)->GetPolygonCount() * 3;
		//Model->vertexNumForMaterial.emplace_back(offset);
	}
}

void FBXLoader::LoadConvertFBXNormal(std::vector<FbxMesh*>* Meshes, Model_HSM* Model)
{
	int offset = 0;
	for (int meshNum = 0; meshNum < Meshes->size(); meshNum++)
	{
		FbxArray<FbxVector4> normals;
		//�@�����X�g�̎擾
		Meshes->at(meshNum)->GetPolygonVertexNormals(normals);

		//�@���ݒ�
		for (int i = 0; i < normals.Size(); i++)
		{
			Model->vertices[i + offset].normal.x = normals[i][0];
			Model->vertices[i + offset].normal.y = normals[i][2];
			Model->vertices[i + offset].normal.z = normals[i][1];
		}

		offset += normals.Size();
	}
}

void FBXLoader::LoadConvertFBXUV(FbxMesh* Mesh, Model_HSM* Model)
{
	//UV�f�[�^�̐�
	int uvCount = Mesh->GetTextureUVCount();

	//uvset�̖��O�ۑ��p
	FbxStringList uvsetNames;
	//UVSet�̖��O���X�g���擾
	Mesh->GetUVSetNames(uvsetNames);

	std::vector<float2>uvArray;

	for (int i = 0; i < Mesh->GetPolygonCount(); ++i)
	{
		for (int j = 0; j < Mesh->GetPolygonSize(i); ++j)
		{
			if (0 < uvCount)
			{
				for (int k = 0; k < uvsetNames.GetCount(); ++k)
				{
					FbxVector2 uvs;
					bool lUnmappedUV;
					if (Mesh->GetPolygonVertexUV(i, j, uvsetNames[k], uvs, lUnmappedUV))
					{
						uvArray.push_back({ (float)uvs[0],(float)uvs[1] });
					}
				}
			}
		}
	}

	for (int i = 0; i < uvArray.size(); ++i)
	{
		Model->vertices[i].uv = uvArray[i];
	}

	//FbxArray<FbxVector2>uvBuffer;
	////UVSet�̖��O����UVSet���擾
	//Mesh->GetPolygonVertexUVs(uvsetNames.GetStringAt(0), uvBuffer);

	//for (int i = 0; i < uvBuffer.Size(); i++)
	//{
	//	Model->vertices[i].uv.x = *uvBuffer.GetAt(i);
	//	Model->vertices[i].uv.y = (1.0f - *uvBuffer.GetAt(i) + sizeof(FbxVector2) / 2);
	//}
}

//void FBXLoader::LoadConvertFBXClusterAndBone(FbxMesh* Mesh, Model_HSM* Model, std::vector<FbxCluster*> Clusters)
//{
//	//�X�L���̐����擾
//	int skinCount = Mesh->GetDeformerCount(FbxDeformer::eSkin);
//
//	for (int i = 0; i < skinCount; ++i)
//	{
//		//i�Ԗڂ̃X�L�����擾
//		FbxSkin* skin = (FbxSkin*)Mesh->GetDeformer(i, FbxDeformer::eSkin);
//
//		//�N���X�^�[�̐����擾
//		int clusterNum = skin->GetClusterCount();
//
//		for (int j = 0; j < clusterNum; ++j)
//		{
//			//j�Ԗڂ̃N���X�^���擾
//			FbxCluster* cluster = skin->GetCluster(j);
//			string clusterName = cluster->GetName();
//
//			//�{�[��
//			Model->skeleton.bones.emplace_back();
//			Model->skeleton.bones[j].name = cluster->GetName();
//
//			printf("%d�{�ڂ̃{�[���F%s\n", j, clusterName.c_str());
//
//			//�N���X�^�[���{�[���̏��Ƃ��ĕۑ�
//			//LoadConvertFBXBone(cluster, j, Model, BoneAffectTable);
//			Clusters.emplace_back(cluster);
//
//			//const FbxMatrix& globalTransform = cluster->GetLink()->EvaluateGlobalTransform();
//			FbxAMatrix globalTransform;
//			cluster->GetTransformLinkMatrix(globalTransform);
//			Model->skeleton.bones[j].offsetMat = ConvertToXMMATRIX(globalTransform);
//		}
//	}
//}

void FBXLoader::LoadConvertFBXBone(FbxCluster* Cluster, int BoneIndex, 
	Model_HSM* Model, std::map<int, std::forward_list<BoneAffectInfo>>& BoneAffectTable)
{
	//�Y���{�[�����e����^���钸�_�̐�
	int pointNum = Cluster->GetControlPointIndicesCount();
	//�e����^���钸�_�̃C���f�b�N�X�z��
	int* pointArray = Cluster->GetControlPointIndices();
	//�E�F�C�g�z��
	double* weightArray = Cluster->GetControlPointWeights();

	//�{�[�����e����^���钸�_�̏����擾����
	for (int i = 0; i < pointNum; ++i)
	{
		//���_�C���f�b�N�X�ƃE�F�C�g���擾
		int index = pointArray[i];
		float weight = (float)weightArray[i];

		//�����̏���
		BoneAffectInfo info;
		info.index = BoneIndex;

		if (0.99 < weight)weight = 1.0f;
		if (weight < 0.01)weight = 0.0f;

		info.weight = weight;

		if (info.weight != 0.0f)
		{
			//���_�C���f�b�N�X(�Y��)���ƂɃ��X�g�Ƃ��ĉe�����󂯂�{�[���͊Ǘ����Ă���B
			//�{�[�������v�b�V��
			BoneAffectTable[index].emplace_front(info);
		}
	}
}

void FBXLoader::LoadConvertFBXSkin(FbxMesh* Mesh, Model_HSM* Model, std::map<int, std::forward_list<BoneAffectInfo>>& BoneAffectTable)
{
	//�X�L���̐����擾
	int skinCount = Mesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < skinCount; ++i)
	{
		//i�Ԗڂ̃X�L�����擾
		FbxSkin* skin = (FbxSkin*)Mesh->GetDeformer(i, FbxDeformer::eSkin);

		//�N���X�^�[�̐����擾
		int clusterNum = skin->GetClusterCount();

		for (int j = 0; j < clusterNum; ++j)
		{
			//j�Ԗڂ̃N���X�^���擾
			FbxCluster* cluster = skin->GetCluster(j);
			string clusterName = cluster->GetName();
			
			//�{�[��
			if (Model->skeleton.bones.empty() || Model->skeleton.bones.size() - 1 < j || Model->skeleton.bones[j].name != clusterName)
			{
				Model->skeleton.bones.emplace_back();
				Model->skeleton.bones[j].name = clusterName;
			}

			printf("%d�{�ڂ̃{�[���F%s\n", j, clusterName.c_str());

			//�N���X�^�[���{�[���̏��Ƃ��ĕۑ�
			LoadConvertFBXBone(cluster, j, Model,BoneAffectTable);

			//const FbxMatrix& globalTransform = cluster->GetLink()->EvaluateGlobalTransform();
			FbxAMatrix globalTransform;
			cluster->GetTransformLinkMatrix(globalTransform);
			Model->skeleton.bones[j].offsetMat = ConvertToXMMATRIX(globalTransform);
		}
	}
}

void FBXLoader::LoadConvertFBXMaterial(std::string Dir, FbxMesh* Mesh, Model_HSM* Model)
{
	FbxNode* meshRootNode = Mesh->GetNode();
	//�}�e���A���̐�
	int materialNum = meshRootNode->GetMaterialCount();
	if (materialNum == 0)return;

	//�}�e���A���̏����擾
	for (int i = 0; i < materialNum; ++i)
	{
		FbxSurfaceMaterial* material = meshRootNode->GetMaterial(i);

		if (material == 0)continue;

		//�����̃}�e���A�������łɂ�������X���[
		auto result = find_if(Model->materials.begin(), Model->materials.end(), [material](Material_HSMA hsma)
			{
				return hsma.head.name == material->GetName();
			}
		);
		if (result != Model->materials.end())continue;

		Model->materials.emplace_back();

		//�}�e���A�����
		Model->materials.back().head.name = material->GetName();

		//�A���r�G���g
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
		if (prop.IsValid())
		{
			FbxColor ambientRGB = prop.Get<FbxColor>();
			Model->materials.back().material.material.ambient.x = ambientRGB.mRed;
			Model->materials.back().material.material.ambient.y = ambientRGB.mGreen;
			Model->materials.back().material.material.ambient.z = ambientRGB.mBlue;
		}

		//�f�B�q���[�Y
		prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (prop.IsValid())
		{
			FbxColor diffuseRGB = prop.Get<FbxColor>();
			Model->materials.back().material.material.diffuse.x = diffuseRGB.mRed;
			Model->materials.back().material.material.diffuse.y = diffuseRGB.mGreen;
			Model->materials.back().material.material.diffuse.z = diffuseRGB.mBlue;
			Model->materials.back().material.material.diffuse.w = diffuseRGB.mAlpha;
		}

		//�f�B�q���[�Y���e�N�X�`���̏��������Ă���
		//FbxFileTexture���擾
		FbxFileTexture* tex = nullptr;
		int textureNum = prop.GetSrcObjectCount<FbxFileTexture>();
		if (0 < textureNum)
		{
			//prop����FbxFileTexture���擾
			tex = prop.GetSrcObject<FbxFileTexture>(0);
		}
		else
		{
			//���s������}���`�e�N�X�`���̉\�����l����FbxLayeredTexture���w��
			//FbxLayeredTexture����FbxFileTexture���擾
			int layerNum = prop.GetSrcObjectCount<FbxLayeredTexture>();
			if (0 < layerNum)
			{
				tex = prop.GetSrcObject<FbxFileTexture>(0);
			}
		}
		if (tex != nullptr)
		{
			string texFileName = GetFileNameFromPath(tex->GetRelativeFileName());
			Model->materials.back().normalTexHandle = SRVDescMgr::Instance()->CreateSRV(Dir + texFileName);
		}

		//�X�y�L�����[
		prop = material->FindProperty(FbxSurfaceMaterial::sSpecular);
		if (prop.IsValid())
		{
			FbxColor specularRGB = prop.Get<FbxColor>();
			Model->materials.back().material.material.specular.x = specularRGB.mRed;
			Model->materials.back().material.material.specular.y = specularRGB.mGreen;
			Model->materials.back().material.material.specular.z = specularRGB.mBlue;
		}
		prop = material->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
		if (prop.IsValid())
		{
			float factor = prop.Get<float>();
			Model->materials.back().material.material.specular_c = factor;
		}

		//�A���t�@
		prop = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
		if(prop.IsValid())
		{
			float trans = prop.Get<float>();
			Model->materials.back().material.material.alpha = 1 - trans;
		}

		//���ۗ�
		//�A���t�@
		//if (!material->GetClassId().Is(FbxSurfaceLambert::ClassId))		//Phong�Ȃ�
		//{
		//}
	}

	//// �}�e���A����������ΏI���
	//if (Mesh->GetElementMaterialCount() == 0)
	//{
	//	return;
	//}

	//// Mesh���̃}�e���A�������擾
	//FbxLayerElementMaterial* material = Mesh->GetElementMaterial(0);
	//// FbxSurfaceMaterial�̃C���f�b�N�X�o�b�t�@����C���f�b�N�X���擾
	//int index = material->GetIndexArray().GetAt(0);
	//// GetSrcObject<FbxSurfaceMaterial>�Ń}�e���A���擾
	//FbxSurfaceMaterial* surface_material = Mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
	//// �}�e���A�����̕ۑ�
	//if (surface_material != nullptr)
	//{
	//	string str = surface_material->GetName();
	//	int k = 0;
	//}
	//else
	//{
	//	int k = 0;
	//	//mesh_data.m_MaterialName = "";
	//}
}

Model_HSM* FBXLoader::LoadFBXModel(std::string Dir, std::string FileName, bool Culling)
{
	//�}�l�[�W������
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXManager�̐����Ɏ��s");
		return nullptr;
	}

	//IOSetting�𐶐�
	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);

	//�C���|�[�^����
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXImporter�̐����Ɏ��s");
		fbxManager->Destroy();
		return nullptr;
	}
	//�t�@�C��������������
	if (fbxImporter->Initialize((Dir + FileName).c_str(), -1, fbxManager->GetIOSettings()) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBX�t�@�C���̏������Ɏ��s");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return nullptr;
	}

	//�V�[���I�u�W�F�N�g����
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	if (fbxScene == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXScene�̐����Ɏ��s");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return nullptr;
	}

	//�V�[���I�u�W�F�N�g��fbx�t�@�C�����̏��𗬂�����
	if (fbxImporter->Import(fbxScene) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBX�t�@�C���̃C���|�[�g�Ɏ��s");
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return nullptr;
	}

	//�V�[�����̃m�[�h�̃|���S����S�ĎO�p�`�ɂ���
	FbxGeometryConverter converter(fbxManager);
	//�SFbxMesh���}�e���A���P�ʂŕ���
	converter.SplitMeshesPerMaterial(fbxScene, true);
	//�|���S�����O�p�`�ɂ���
	converter.Triangulate(fbxScene, true);

	//�V�[���𗬂����񂾂�Importer�͊J������OK
	fbxImporter->Destroy();

	auto Model = new  Model_HSM(Culling);

	printf("\nHSM���f���̃��[�h===================================\n");

	std::vector<FbxMesh*>meshes;
	GetMesh(fbxScene->GetRootNode(), &meshes);

	//int nodeCount = fbxScene->GetRootNode()->GetChildCount();

	//std::map<std::string, FbxNode*>meshList;
	//CollectMeshNode(fbxScene->GetRootNode(), meshList);

	for (int i = 0; i < meshes.size(); i++)
	{
		string meshName = meshes[i]->GetName();

		printf("���b�V�����F%s\n", meshName.c_str());
		//�{�[�������_�ɗ^����e���Ɋւ�����e�[�u��
		//< ���_�C���f�b�N�X�A���(�ϒ��A�S�܂�)>
		//�T�ȏ�ǂݍ��܂�Ă����ꍇ�A���_�ǂݍ��ݎ��ɖ��������
		std::map<int, std::forward_list<BoneAffectInfo>>boneAffectTable;

		//�{�[��
		LoadConvertFBXSkin(meshes[i], Model, boneAffectTable);

		//���_
		LoadConvertFBXVertex(meshes[i], Model, boneAffectTable);
		//UV
		LoadConvertFBXUV(meshes[i], Model);

		//�}�e���A��
		LoadConvertFBXMaterial(Dir, meshes[i], Model);

		if (meshes[i]->GetElementMaterialCount() != 0)
		{
			FbxLayerElementMaterial* element = meshes[i]->GetElementMaterial(0);
			//(FBX��ł�)�}�e���A���̃C���f�b�N�X�擾
			int materialIndexNum = element->GetIndexArray().GetAt(0);
			//(FBX��ł�)�}�e���A���擾
			FbxSurfaceMaterial* surface_material = meshes[i]->GetNode()->GetSrcObject<FbxSurfaceMaterial>(materialIndexNum);
			//���b�V���ɓK�p����}�e���A�����擾
			string name = surface_material->GetName();


			//HSM�̃}�e���A���Ŗ��O�����v����}�e���A���̃C���f�b�N�X�擾
			for (int j = 0; j <  Model->materials.size(); ++j)
			{
				if ( Model->materials[j].head.name == name)
				{
					 Model->meshes.emplace_back();
					 Model->meshes.back().name = meshName;
					 Model->meshes.back().materialIndex = j;
					 Model->meshes.back().vertexNum = meshes[i]->GetPolygonCount() * 3;
					break;
				}
			}
		}
	}
	//�C���f�b�N�X
	LoadConvertFBXIndex(&meshes, Model);
	//�@��
	LoadConvertFBXNormal(&meshes, Model);

	(Model)->CreateBoneTree();
	(Model)->CreateBuff();

	if (fbxScene != nullptr)fbxScene->Destroy();
	if (fbxManager != nullptr)fbxManager->Destroy();

	return Model;
}

void FBXLoader::GetSkeletonNode(FbxNode* RootNode, std::vector<FbxNode*>* SkeletonNodes)
{
	int childCount = RootNode->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* child = RootNode->GetChild(i);	//�q�m�[�h���擾
		int attributes = child->GetNodeAttributeCount();
		for (int num = 0; num < attributes; num++)
		{
			FbxNodeAttribute* info = child->GetNodeAttributeByIndex(num);
			FbxNodeAttribute::EType type = info->GetAttributeType();

			if (type == FbxNodeAttribute::EType::eSkeleton)
			{
				SkeletonNodes->emplace_back(child);
			}
		}
		GetSkeletonNode(child, SkeletonNodes);
	}
}

void FBXLoader::LoadFBXAnimation(std::string Dir, std::string FileName, Action** ActionTarget, AFFECT_MODE AffectMode, HSM::Skeleton* Skeleton)
{
#pragma region ����
	//�}�l�[�W������
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXManager�̐����Ɏ��s");
		return;
	}

	//IOSetting�𐶐�
	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);

	//�C���|�[�^����
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXImporter�̐����Ɏ��s");
		fbxManager->Destroy();
		return;
	}
	//�t�@�C��������������
	if (fbxImporter->Initialize((Dir + FileName).c_str(), -1, fbxManager->GetIOSettings()) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBX�t�@�C���̏������Ɏ��s");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		DebugBreak();
		return;
	}

	//�V�[���I�u�W�F�N�g����
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	if (fbxScene == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXScene�̐����Ɏ��s");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return;
	}

	//�V�[���I�u�W�F�N�g��fbx�t�@�C�����̏��𗬂�����
	if (fbxImporter->Import(fbxScene) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBX�t�@�C���̃C���|�[�g�Ɏ��s");
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return;
	}
#pragma endregion

	if (*ActionTarget != nullptr)
	{
		printf("FBX�A�j���[�V�����̐����悪nullptr�ł͂Ȃ������̂�delete���Ă���g���܂�");
		delete (*ActionTarget);
	}
	*ActionTarget = new Action(AffectMode);
	(*ActionTarget)->name = FileName;
	(*ActionTarget)->dir = Dir;

	//LoadFbxScene(fbxScene, *ActionTarget);

	std::vector<FbxNode*>skeletonNodes;
	GetSkeletonNode(fbxScene->GetRootNode(), &skeletonNodes);

	int animStackCount = fbxImporter->GetAnimStackCount();
	for (int i = 0; i < animStackCount; ++i)
	{
		printf("\nAnimStack�̃��[�h=====================\n");
		FbxTakeInfo* fbxTakeInfo = fbxImporter->GetTakeInfo(i);
		FbxLongLong start = fbxTakeInfo->mLocalTimeSpan.GetStart().Get();
		FbxLongLong stop = fbxTakeInfo->mLocalTimeSpan.GetStop().Get();
		FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
		int totalFrame = (stop - start) / oneFrameValue;
		(*ActionTarget)->total = totalFrame;
		printf("�g�[�^���t���[���F%d\n", totalFrame);


		for (int l = 0; l < skeletonNodes.size(); ++l)
		{
			for (int k = 0; k < Skeleton->bones.size(); ++k)
			{
				if (Skeleton->bones[k].name == skeletonNodes[l]->GetName())
				{
					(*ActionTarget)->animations.emplace_back();
					(*ActionTarget)->animations.back().boneName = skeletonNodes[l]->GetName();
					//printf("\n�{�[�����F%s==========================================\n", skeletonNodes[l]->GetName());
					printf("\n�{�[�����F%s\n", skeletonNodes[l]->GetName());
					
					for (int j = 0; j < totalFrame; ++j)
					{
						FbxTime time = start + oneFrameValue * j;
						FbxMatrix m = skeletonNodes[l]->EvaluateGlobalTransform(time);
						XMMATRIX grobalTransform = ConvertToXMMATRIX(m);

						AnimationInfo::Key key;
						key.frame = j;
						key.value = grobalTransform;

						/*printf("�@�L�[�t���[��\n");
						printf("�@frame�F%d\n", j);
						printf("�@value �F%.3f %.3f %.3f %3f\n", 
							key.value.r[0].m128_f32[0], key.value.r[0].m128_f32[1],key.value.r[0].m128_f32[2], key.value.r[0].m128_f32[3]);
						printf("�@     �F%.3f %.3f %.3f %3f\n",
							key.value.r[1].m128_f32[0], key.value.r[1].m128_f32[1], key.value.r[1].m128_f32[2], key.value.r[1].m128_f32[3]);
						printf("�@     �F%.3f %.3f %.3f %3f\n",
							key.value.r[2].m128_f32[0], key.value.r[2].m128_f32[1], key.value.r[2].m128_f32[2], key.value.r[2].m128_f32[3]);
						printf("�@     �F%.3f %.3f %.3f %3f\n",
							key.value.r[3].m128_f32[0], key.value.r[3].m128_f32[1], key.value.r[3].m128_f32[2], key.value.r[3].m128_f32[3]);*/

						(*ActionTarget)->animations.back().keys.emplace_back(key);
					}
					break;
				}
			}
		}
	}

	if (fbxScene != nullptr)fbxScene->Destroy();
	if (fbxManager != nullptr)fbxManager->Destroy();
}
