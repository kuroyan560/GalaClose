#include "CenterPillar.h"
#include"../../FbxLoader.h"

CenterPillar::CenterPillar()
{
	Model_HSM* fbxModel;
	fbxModel = FBXLoader::LoadFBXModel("resource/model/center/", "center.fbx", true);

	model.reset(new Model_HSM_Normal(fbxModel));
}

void CenterPillar::Init()
{
	model.get()->Init();
}

void CenterPillar::Update()
{
}

void CenterPillar::Draw()
{
	model.get()->Draw();
}

void CenterPillar::Finalize()
{
}
