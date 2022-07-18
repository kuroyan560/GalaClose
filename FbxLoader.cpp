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
		FbxNode* child = Node->GetChild(i);	//子ノードを取得
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
	//影響データ表が空じゃない
	if (!BoneAffectTable[Index].empty())
	{
		//適用されるボーンの数
		int count = 0;

		//該当インデックスの影響データ一覧を参照
		for (auto itr = BoneAffectTable[Index].begin(); itr != BoneAffectTable[Index].end(); ++itr)
		{
			for (int i = 0; i < 4; ++i)
			{
				//対象の頂点のボーンデータで空な領域にデータを保存
				if (VertexPtr->boneIndex[i] == -1)	//ボーン未登録
				{
					VertexPtr->boneIndex[i] = itr->index;
					VertexPtr->boneWeight[i] = itr->weight;
					break;
				}
			}
			count++;
		}

		//１つの頂点につき最大４つのボーンを適用出来ないため、５つ目以降の情報は無視されていることを警告
		if (4 < count)
		{
			WinApp::Instance()->
				CreateMessageWindow("WARNING", "FBXLoaderの頂点読み込みにて５つ以上のボーンが適用されました。５つ目以降は無視されます");
		}
	}
}

void FBXLoader::LoadConvertFBXVertex(FbxMesh* Mesh, Model_HSM* Model, std::map<int, std::forward_list<BoneAffectInfo>>& Info)
{
	//頂点バッファの取得
	FbxVector4* vertices = Mesh->GetControlPoints();
	//インデックスバッファの取得
	int* indices = Mesh->GetPolygonVertices();
	//頂点座標の数の取得
	int polygonVertexCount = Mesh->GetPolygonVertexCount();

	for (int i = 0; i < polygonVertexCount; i++)
	{
		Model->vertices.emplace_back();

		//インデックスバッファから頂点番号を取得
		int index = indices[i];

		float3 pos;
		//頂点座標リストから座標を取得
		pos.x = vertices[index][0];
		pos.y = vertices[index][1];
		pos.z = vertices[index][2];

		Model->vertices.back().pos = pos;

		//保存しておいたボーンの対応表から頂点に影響データを取得
		SetBoneAffectToVertex(Info, &(Model->vertices.back()), index);
	}
}

void FBXLoader::LoadConvertFBXIndex(std::vector<FbxMesh*>* Meshes, Model_HSM* Model)
{
	int offset = 0;
	for (int meshNum = 0; meshNum < Meshes->size(); meshNum++)
	{
		//ポリゴンの数だけ連番として保存する
		for (int i = 0; i < Meshes->at(meshNum)->GetPolygonCount(); i++)
		{
			//左手系（右周り）
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
		//法線リストの取得
		Meshes->at(meshNum)->GetPolygonVertexNormals(normals);

		//法線設定
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
	//UVデータの数
	int uvCount = Mesh->GetTextureUVCount();

	//uvsetの名前保存用
	FbxStringList uvsetNames;
	//UVSetの名前リストを取得
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
	////UVSetの名前からUVSetを取得
	//Mesh->GetPolygonVertexUVs(uvsetNames.GetStringAt(0), uvBuffer);

	//for (int i = 0; i < uvBuffer.Size(); i++)
	//{
	//	Model->vertices[i].uv.x = *uvBuffer.GetAt(i);
	//	Model->vertices[i].uv.y = (1.0f - *uvBuffer.GetAt(i) + sizeof(FbxVector2) / 2);
	//}
}

//void FBXLoader::LoadConvertFBXClusterAndBone(FbxMesh* Mesh, Model_HSM* Model, std::vector<FbxCluster*> Clusters)
//{
//	//スキンの数を取得
//	int skinCount = Mesh->GetDeformerCount(FbxDeformer::eSkin);
//
//	for (int i = 0; i < skinCount; ++i)
//	{
//		//i番目のスキンを取得
//		FbxSkin* skin = (FbxSkin*)Mesh->GetDeformer(i, FbxDeformer::eSkin);
//
//		//クラスターの数を取得
//		int clusterNum = skin->GetClusterCount();
//
//		for (int j = 0; j < clusterNum; ++j)
//		{
//			//j番目のクラスタを取得
//			FbxCluster* cluster = skin->GetCluster(j);
//			string clusterName = cluster->GetName();
//
//			//ボーン
//			Model->skeleton.bones.emplace_back();
//			Model->skeleton.bones[j].name = cluster->GetName();
//
//			printf("%d本目のボーン：%s\n", j, clusterName.c_str());
//
//			//クラスターをボーンの情報として保存
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
	//該当ボーンが影響を与える頂点の数
	int pointNum = Cluster->GetControlPointIndicesCount();
	//影響を与える頂点のインデックス配列
	int* pointArray = Cluster->GetControlPointIndices();
	//ウェイト配列
	double* weightArray = Cluster->GetControlPointWeights();

	//ボーンが影響を与える頂点の情報を取得する
	for (int i = 0; i < pointNum; ++i)
	{
		//頂点インデックスとウェイトを取得
		int index = pointArray[i];
		float weight = (float)weightArray[i];

		//それらの情報を
		BoneAffectInfo info;
		info.index = BoneIndex;

		if (0.99 < weight)weight = 1.0f;
		if (weight < 0.01)weight = 0.0f;

		info.weight = weight;

		if (info.weight != 0.0f)
		{
			//頂点インデックス(添字)ごとにリストとして影響を受けるボーンは管理している。
			//ボーン情報をプッシュ
			BoneAffectTable[index].emplace_front(info);
		}
	}
}

void FBXLoader::LoadConvertFBXSkin(FbxMesh* Mesh, Model_HSM* Model, std::map<int, std::forward_list<BoneAffectInfo>>& BoneAffectTable)
{
	//スキンの数を取得
	int skinCount = Mesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < skinCount; ++i)
	{
		//i番目のスキンを取得
		FbxSkin* skin = (FbxSkin*)Mesh->GetDeformer(i, FbxDeformer::eSkin);

		//クラスターの数を取得
		int clusterNum = skin->GetClusterCount();

		for (int j = 0; j < clusterNum; ++j)
		{
			//j番目のクラスタを取得
			FbxCluster* cluster = skin->GetCluster(j);
			string clusterName = cluster->GetName();
			
			//ボーン
			if (Model->skeleton.bones.empty() || Model->skeleton.bones.size() - 1 < j || Model->skeleton.bones[j].name != clusterName)
			{
				Model->skeleton.bones.emplace_back();
				Model->skeleton.bones[j].name = clusterName;
			}

			printf("%d本目のボーン：%s\n", j, clusterName.c_str());

			//クラスターをボーンの情報として保存
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
	//マテリアルの数
	int materialNum = meshRootNode->GetMaterialCount();
	if (materialNum == 0)return;

	//マテリアルの情報を取得
	for (int i = 0; i < materialNum; ++i)
	{
		FbxSurfaceMaterial* material = meshRootNode->GetMaterial(i);

		if (material == 0)continue;

		//同名のマテリアルがすでにあったらスルー
		auto result = find_if(Model->materials.begin(), Model->materials.end(), [material](Material_HSMA hsma)
			{
				return hsma.head.name == material->GetName();
			}
		);
		if (result != Model->materials.end())continue;

		Model->materials.emplace_back();

		//マテリアル解析
		Model->materials.back().head.name = material->GetName();

		//アンビエント
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
		if (prop.IsValid())
		{
			FbxColor ambientRGB = prop.Get<FbxColor>();
			Model->materials.back().material.material.ambient.x = ambientRGB.mRed;
			Model->materials.back().material.material.ambient.y = ambientRGB.mGreen;
			Model->materials.back().material.material.ambient.z = ambientRGB.mBlue;
		}

		//ディヒューズ
		prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (prop.IsValid())
		{
			FbxColor diffuseRGB = prop.Get<FbxColor>();
			Model->materials.back().material.material.diffuse.x = diffuseRGB.mRed;
			Model->materials.back().material.material.diffuse.y = diffuseRGB.mGreen;
			Model->materials.back().material.material.diffuse.z = diffuseRGB.mBlue;
			Model->materials.back().material.material.diffuse.w = diffuseRGB.mAlpha;
		}

		//ディヒューズがテクスチャの情報を持っている
		//FbxFileTextureを取得
		FbxFileTexture* tex = nullptr;
		int textureNum = prop.GetSrcObjectCount<FbxFileTexture>();
		if (0 < textureNum)
		{
			//propからFbxFileTextureを取得
			tex = prop.GetSrcObject<FbxFileTexture>(0);
		}
		else
		{
			//失敗したらマルチテクスチャの可能性を考えてFbxLayeredTextureを指定
			//FbxLayeredTextureからFbxFileTextureを取得
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

		//スペキュラー
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

		//アルファ
		prop = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
		if(prop.IsValid())
		{
			float trans = prop.Get<float>();
			Model->materials.back().material.material.alpha = 1 - trans;
		}

		//※保留
		//アルファ
		//if (!material->GetClassId().Is(FbxSurfaceLambert::ClassId))		//Phongなら
		//{
		//}
	}

	//// マテリアルが無ければ終わり
	//if (Mesh->GetElementMaterialCount() == 0)
	//{
	//	return;
	//}

	//// Mesh側のマテリアル情報を取得
	//FbxLayerElementMaterial* material = Mesh->GetElementMaterial(0);
	//// FbxSurfaceMaterialのインデックスバッファからインデックスを取得
	//int index = material->GetIndexArray().GetAt(0);
	//// GetSrcObject<FbxSurfaceMaterial>でマテリアル取得
	//FbxSurfaceMaterial* surface_material = Mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
	//// マテリアル名の保存
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
	//マネージャ生成
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXManagerの生成に失敗");
		return nullptr;
	}

	//IOSettingを生成
	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);

	//インポータ生成
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXImporterの生成に失敗");
		fbxManager->Destroy();
		return nullptr;
	}
	//ファイルを初期化する
	if (fbxImporter->Initialize((Dir + FileName).c_str(), -1, fbxManager->GetIOSettings()) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXファイルの初期化に失敗");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return nullptr;
	}

	//シーンオブジェクト生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	if (fbxScene == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXSceneの生成に失敗");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return nullptr;
	}

	//シーンオブジェクトにfbxファイル内の情報を流し込む
	if (fbxImporter->Import(fbxScene) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXファイルのインポートに失敗");
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return nullptr;
	}

	//シーン内のノードのポリゴンを全て三角形にする
	FbxGeometryConverter converter(fbxManager);
	//全FbxMeshをマテリアル単位で分割
	converter.SplitMeshesPerMaterial(fbxScene, true);
	//ポリゴンを三角形にする
	converter.Triangulate(fbxScene, true);

	//シーンを流し込んだらImporterは開放してOK
	fbxImporter->Destroy();

	auto Model = new  Model_HSM(Culling);

	printf("\nHSMモデルのロード===================================\n");

	std::vector<FbxMesh*>meshes;
	GetMesh(fbxScene->GetRootNode(), &meshes);

	//int nodeCount = fbxScene->GetRootNode()->GetChildCount();

	//std::map<std::string, FbxNode*>meshList;
	//CollectMeshNode(fbxScene->GetRootNode(), meshList);

	for (int i = 0; i < meshes.size(); i++)
	{
		string meshName = meshes[i]->GetName();

		printf("メッシュ名：%s\n", meshName.c_str());
		//ボーンが頂点に与える影響に関する情報テーブル
		//< 頂点インデックス、情報(可変長、４個まで)>
		//５つ以上読み込まれていた場合、頂点読み込み時に無視される
		std::map<int, std::forward_list<BoneAffectInfo>>boneAffectTable;

		//ボーン
		LoadConvertFBXSkin(meshes[i], Model, boneAffectTable);

		//頂点
		LoadConvertFBXVertex(meshes[i], Model, boneAffectTable);
		//UV
		LoadConvertFBXUV(meshes[i], Model);

		//マテリアル
		LoadConvertFBXMaterial(Dir, meshes[i], Model);

		if (meshes[i]->GetElementMaterialCount() != 0)
		{
			FbxLayerElementMaterial* element = meshes[i]->GetElementMaterial(0);
			//(FBX上での)マテリアルのインデックス取得
			int materialIndexNum = element->GetIndexArray().GetAt(0);
			//(FBX上での)マテリアル取得
			FbxSurfaceMaterial* surface_material = meshes[i]->GetNode()->GetSrcObject<FbxSurfaceMaterial>(materialIndexNum);
			//メッシュに適用するマテリアル名取得
			string name = surface_material->GetName();


			//HSMのマテリアルで名前が合致するマテリアルのインデックス取得
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
	//インデックス
	LoadConvertFBXIndex(&meshes, Model);
	//法線
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
		FbxNode* child = RootNode->GetChild(i);	//子ノードを取得
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
#pragma region 準備
	//マネージャ生成
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXManagerの生成に失敗");
		return;
	}

	//IOSettingを生成
	FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);

	//インポータ生成
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXImporterの生成に失敗");
		fbxManager->Destroy();
		return;
	}
	//ファイルを初期化する
	if (fbxImporter->Initialize((Dir + FileName).c_str(), -1, fbxManager->GetIOSettings()) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXファイルの初期化に失敗");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		DebugBreak();
		return;
	}

	//シーンオブジェクト生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	if (fbxScene == nullptr)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXSceneの生成に失敗");
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return;
	}

	//シーンオブジェクトにfbxファイル内の情報を流し込む
	if (fbxImporter->Import(fbxScene) == false)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "FBXファイルのインポートに失敗");
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return;
	}
#pragma endregion

	if (*ActionTarget != nullptr)
	{
		printf("FBXアニメーションの生成先がnullptrではなかったのでdeleteしてから使います");
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
		printf("\nAnimStackのロード=====================\n");
		FbxTakeInfo* fbxTakeInfo = fbxImporter->GetTakeInfo(i);
		FbxLongLong start = fbxTakeInfo->mLocalTimeSpan.GetStart().Get();
		FbxLongLong stop = fbxTakeInfo->mLocalTimeSpan.GetStop().Get();
		FbxLongLong oneFrameValue = FbxTime::GetOneFrameValue(FbxTime::eFrames60);
		int totalFrame = (stop - start) / oneFrameValue;
		(*ActionTarget)->total = totalFrame;
		printf("トータルフレーム：%d\n", totalFrame);


		for (int l = 0; l < skeletonNodes.size(); ++l)
		{
			for (int k = 0; k < Skeleton->bones.size(); ++k)
			{
				if (Skeleton->bones[k].name == skeletonNodes[l]->GetName())
				{
					(*ActionTarget)->animations.emplace_back();
					(*ActionTarget)->animations.back().boneName = skeletonNodes[l]->GetName();
					//printf("\nボーン名：%s==========================================\n", skeletonNodes[l]->GetName());
					printf("\nボーン名：%s\n", skeletonNodes[l]->GetName());
					
					for (int j = 0; j < totalFrame; ++j)
					{
						FbxTime time = start + oneFrameValue * j;
						FbxMatrix m = skeletonNodes[l]->EvaluateGlobalTransform(time);
						XMMATRIX grobalTransform = ConvertToXMMATRIX(m);

						AnimationInfo::Key key;
						key.frame = j;
						key.value = grobalTransform;

						/*printf("　キーフレーム\n");
						printf("　frame：%d\n", j);
						printf("　value ：%.3f %.3f %.3f %3f\n", 
							key.value.r[0].m128_f32[0], key.value.r[0].m128_f32[1],key.value.r[0].m128_f32[2], key.value.r[0].m128_f32[3]);
						printf("　     ：%.3f %.3f %.3f %3f\n",
							key.value.r[1].m128_f32[0], key.value.r[1].m128_f32[1], key.value.r[1].m128_f32[2], key.value.r[1].m128_f32[3]);
						printf("　     ：%.3f %.3f %.3f %3f\n",
							key.value.r[2].m128_f32[0], key.value.r[2].m128_f32[1], key.value.r[2].m128_f32[2], key.value.r[2].m128_f32[3]);
						printf("　     ：%.3f %.3f %.3f %3f\n",
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
