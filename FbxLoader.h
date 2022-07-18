#pragma once
#include<string>
#include<fbxsdk.h>
#include"Model_HSM.h"
#include"AnimationInfo.h"

class Action;

static class FBXLoader
{
	struct BoneAffectInfo
	{
		signed short index;
		float weight;
	};

	static void GetMesh(FbxNode* Node, std::vector<FbxMesh*>* Mesh);
	//void CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list);
	static void SetBoneAffectToVertex(std::map<int, std::forward_list<BoneAffectInfo>>& Info, HSM::Vertex* VertexPtr, int Index);
	static void LoadConvertFBXVertex(FbxMesh* Mesh, Model_HSM* Model, std::map<int, std::forward_list<BoneAffectInfo>>& AffectBoneTable);
	static void LoadConvertFBXUV(FbxMesh* Mesh, Model_HSM* Model);
	//static void LoadConvertFBXClusterAndBone(FbxMesh* Mesh, Model_HSM* Model, std::vector<FbxCluster*>Clusters);
	static void LoadConvertFBXBone(FbxCluster* Cluster, int BoneIndex, Model_HSM* Mode, std::map<int, std::forward_list<BoneAffectInfo>>& AffectBoneTable);
	static void LoadConvertFBXSkin(FbxMesh* Mesh, Model_HSM* Model, std::map<int, std::forward_list<BoneAffectInfo>>& AffectBoneTable);
	static void LoadConvertFBXIndex(std::vector<FbxMesh*>* Meshes, Model_HSM* Model);
	static void LoadConvertFBXNormal(std::vector<FbxMesh*>* Meshes, Model_HSM* Model);
	static void LoadConvertFBXMaterial(std::string Dir, FbxMesh* Mesh, Model_HSM* Model);

	static void GetSkeletonNode(FbxNode* RootNode, std::vector<FbxNode*>* SkeletonNodes);

public:
	static Model_HSM* LoadFBXModel(std::string Dir, std::string FileName, bool Culling = true);
	static void LoadFBXAnimation(std::string Dir, std::string FileName, Action** ActionTarget, AnimationInfo::AFFECT_MODE AffectMode, HSM::Skeleton* Skeleton);
};