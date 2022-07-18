#include "Action.h"
#include"Model_HSM.h"
#include"FbxLoader.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include<stdlib.h>

using namespace HSM;
std::vector<Action*>Action::staticActions;
const std::string Action::EXTENTION = ".anim";

//void Action::GetValue(float& t, float3& Target, VALUE_MODE ValueMode, std::vector<AnimationInfo::Animation>::iterator& AnimationItr)
//{
//	std::vector<float>pointArray;
//
//	//X
//	for (auto itr = AnimationItr->curves[ValueMode][X].keys.begin(); itr != AnimationItr->curves[ValueMode][X].keys.end(); ++itr)
//	{
//		pointArray.emplace_back(itr->value);
//	}
//	if (!pointArray.empty())
//	{
//		Target.x = MyFunc::GetBezierFromControlPoint(t, pointArray.size(), &pointArray[0]);
//		pointArray.clear();
//	}
//
//	//Y
//	for (auto itr = AnimationItr->curves[ValueMode][Y].keys.begin(); itr != AnimationItr->curves[ValueMode][Y].keys.end(); ++itr)
//	{
//		pointArray.emplace_back(itr->value);
//	}
//	if (!pointArray.empty())
//	{
//		Target.y = MyFunc::GetBezierFromControlPoint(t, pointArray.size(), &pointArray[0]);
//		pointArray.clear();
//	}
//
//	//Z
//	for (auto itr = AnimationItr->curves[ValueMode][Z].keys.begin(); itr != AnimationItr->curves[ValueMode][Z].keys.end(); ++itr)
//	{
//		pointArray.emplace_back(itr->value);
//	}
//	if (!pointArray.empty())
//	{
//		Target.z = MyFunc::GetBezierFromControlPoint(t, pointArray.size(), &pointArray[0]);
//	}
//}

//void Action::GetValue(int& PastFrame, float3& Target, VALUE_MODE ValueMode, std::vector<AnimationInfo::Animation>::iterator& AnimationItr)
//{
//	//X
//	auto itr = std::find_if(AnimationItr->curves[ValueMode][X].keys.begin(), AnimationItr->curves[ValueMode][X].keys.end(),
//		[PastFrame](AnimationInfo::Key key) {return key.frame == PastFrame; });
//	if (itr != AnimationItr->curves[ValueMode][X].keys.end())
//	{
//		if (affectMode == ASSIGN)
//		{
//			Target.x = itr->value;
//		}
//		else if (affectMode == ADDITIONAL)
//		{
//			Target.x += itr->value;
//		}
//	}
//
//	//Y
//	itr = std::find_if(AnimationItr->curves[ValueMode][Y].keys.begin(), AnimationItr->curves[ValueMode][Y].keys.end(),
//		[PastFrame](AnimationInfo::Key key) {return key.frame == PastFrame; });
//	if (itr != AnimationItr->curves[ValueMode][Y].keys.end())
//	{
//		if (affectMode == ASSIGN)
//		{
//			Target.y = itr->value;
//		}
//		else if (affectMode == ADDITIONAL)
//		{
//			Target.y += itr->value;
//		}
//	}
//
//	//Z
//	itr = std::find_if(AnimationItr->curves[ValueMode][Z].keys.begin(), AnimationItr->curves[ValueMode][Z].keys.end(),
//		[PastFrame](AnimationInfo::Key key) {return key.frame == PastFrame; });
//	if (itr != AnimationItr->curves[ValueMode][Z].keys.end())
//	{
//		if (affectMode == ASSIGN)
//		{
//			Target.z = itr->value;
//		}
//		else if (affectMode == ADDITIONAL)
//		{
//			Target.z += itr->value;
//		}
//	}
//}

void Action::ConvertToAnimFile(std::string& FileName, Action* Action)
{
	printf("\nFBXアニメーションからAnimファイルへコンバート\n");
	FILE* fp;
	fopen_s(&fp, FileName.c_str(), "wb");
	if (fp != NULL)
	{
		//アクションのトータルフレーム
		unsigned short totalFrame = Action->total;
		if (fwrite(&totalFrame, sizeof(totalFrame), 1, fp) < 1)
		{
			printf("　トータルフレームの書き込みに失敗\n");
		}

		//アニメーションの数
		unsigned short animationNum = Action->animations.size();
		if (fwrite(&animationNum, sizeof(animationNum), 1, fp) < 1)
		{
			printf("　アニメーションの数の書き込みに失敗\n");
		}

		for (auto itr = Action->animations.begin(); itr != Action->animations.end(); ++itr)
		{
			//ボーンの名前のサイズ
			unsigned short size = itr->boneName.size();
			if (fwrite(&size, sizeof(size), 1, fp) < 1)
			{
				printf("　ボーンの名前のサイズ書き込みに失敗\n");
			}
			//キーの数
			size = itr->keys.size();
			if (fwrite(&size, sizeof(size), 1, fp) < 1)
			{
				printf("　キーの個数書き込みに失敗\n");
			}
			//ボーンの名前本体
			if (fwrite(&itr->boneName[0], itr->boneName.size(), 1, fp) < 1)
			{
				printf("　ボーンの名前書き込みに失敗\n");
			}
			//キー本体
			if (fwrite(&itr->keys[0], sizeof(Key), itr->keys.size(), fp) < 1)
			{
				printf("　キーフレーム本体書き込みに失敗\n");
			}
		}
		fclose(fp);
	}
}

void Action::LoadAnimFile(std::string& FileName, Action* Action)
{
	printf("\nFBXアニメーションの代わりにAnimファイルを読み込み\n");
	FILE* fp;
	fopen_s(&fp, FileName.c_str(), "rb");
	if (fp != NULL)
	{
		//トータルフレーム
		unsigned short totalFrame = -1;
		if (fread(&totalFrame, sizeof(totalFrame), 1, fp) < 1)
		{
			printf("　トータルフレーム読み込みに失敗\n");
		}
		Action->total = totalFrame;

		//アニメーションの数
		unsigned short animationNum = -1;
		if (fread(&animationNum, sizeof(animationNum), 1, fp) < 1)
		{
			printf("　アニメーションの数読み込みに失敗\n");
		}

		for (int i = 0; i < animationNum; ++i)
		{
			//ボーンの名前サイズ
			unsigned short boneNamesize = -1;
			if (fread(&boneNamesize, sizeof(boneNamesize), 1, fp) < 1)
			{
				printf("　ボーンの名前のサイズ読み込みに失敗\n");
			}
			//キーの個数読み込み
			unsigned short keyNum = -1;
			if (fread(&keyNum, sizeof(keyNum), 1, fp) < 1)
			{
				printf("　キーの個数読み込みに失敗\n");
			}

			Animation anim;
			//ボーンの名前読み込み
			anim.boneName.resize(boneNamesize);
			if (fread(&anim.boneName[0], sizeof(char), boneNamesize, fp) < 1)
			{
				printf("　ボーンの名前読み込みに失敗\n");
			}

			//キーフレーム読み込み
			anim.keys.resize(keyNum);
			if (fread(&anim.keys[0], sizeof(Key), keyNum, fp) < 1)
			{
				printf("　キーフレーム読み込みに失敗\n");
			}
			Action->animations.emplace_back(anim);

			if (feof(fp))
			{
				break;
			}
		}
		fclose(fp);
	}
}

void Action::AllActionUpdate()
{
	for (auto itr = staticActions.begin(); itr != staticActions.end(); ++itr)
	{
		(*itr)->Update();
	}
}

void Action::AllActionInit()
{
	for (auto itr = staticActions.begin(); itr != staticActions.end(); ++itr)
	{
		(*itr)->Init();
	}
}

void Action::LoadAction(std::string Dir, std::string FileName, Action** ActionTarget, AFFECT_MODE AffectMode, HSM::Skeleton* Skeleton)
{
	std::string fileName = Dir + FileName + EXTENTION;
	if (!MyFunc::ExistFile(fileName))
	{
		FBXLoader::LoadFBXAnimation(Dir, FileName, ActionTarget, AffectMode, Skeleton);

		//.animファイルとしてコンバートしたものを保存
		ConvertToAnimFile(fileName, *ActionTarget);
	}
	else
	{
		*ActionTarget = new Action(AffectMode);
		(*ActionTarget)->dir = Dir;
		(*ActionTarget)->name = FileName;
		LoadAnimFile(fileName, *ActionTarget);
	}
}

XMMATRIX Action::GetKeyValue(int Past, AnimationInfo::Animation Animation)
{
	for (auto itr = Animation.keys.begin(); itr != Animation.keys.end(); ++itr)
	{
		if (itr->frame == Past)
		{
			return itr->value;
		}
	}
	return XMMatrixIdentity();
}

Action::Action(AFFECT_MODE AffectMode) :affectMode(AffectMode)
{
	staticActions.emplace_back(this);
}

void Action::Init()
{
	isPlay = false;
	past = 0;
	total = 0;
}

void Action::Play(HSM::BoneGroup* BoneGroup, bool Loop, bool NowUpdate)
{
	bool old = isPlay;

	isPlay = true;
	past = 0;
	target = BoneGroup;
	target->nowAnimation++;
	loop = Loop;

	if (NowUpdate)
	{
		this->Update();
	}
}

void Action::Stop()
{
	if(isPlay)
	{
		loop = false;
		isPlay = false;
		target->nowAnimation--;
		target = nullptr;
	}
}

void Action::Update()
{
	//ポーズ
	if (poseTarget != nullptr && !this->isPlay)
	{
		//ボーンごとのアニメーション
		for (auto animation = animations.begin(); animation != animations.end(); ++animation)
		{
			//モデルに存在していないボーンならスルー
			if (poseTarget->skeleton->boneNodeTable.find(animation->boneName) == poseTarget->skeleton->boneNodeTable.end())continue;

			BoneNode& node = poseTarget->skeleton->boneNodeTable[animation->boneName];

			/*	GetValue(past, pos, POS, animation);
				GetValue(past, scale, SCALE, animation);
				GetValue(past, rotate, ROTATE, animation);*/

			XMMATRIX keyValue = GetKeyValue(poseKeyFrame, *animation);
			XMMATRIX value = XMMatrixMultiply(XMMatrixInverse(nullptr, poseTarget->skeleton->bones[node.boneIdx].offsetMat), keyValue);

			if (affectMode == ASSIGN)
			{
				poseTarget->boneMatrices[node.boneIdx] = value;
			}
			else if (affectMode == ADDITIONAL)
			{
				poseTarget->boneMatrices[node.boneIdx] *= value;
			}
		}
		poseTarget = nullptr;
	}

	if (isPlay && target != nullptr)
	{
		//float t = (float)past / (float)total;	//完了率

		//ボーンごとのアニメーション
		for (auto animation = animations.begin(); animation != animations.end(); ++animation)
		{
			//モデルに存在していないボーンならスルー
			if (target->skeleton->boneNodeTable.find(animation->boneName) == target->skeleton->boneNodeTable.end())continue;

			BoneNode& node = target->skeleton->boneNodeTable[animation->boneName];

		/*	GetValue(past, pos, POS, animation);
			GetValue(past, scale, SCALE, animation);
			GetValue(past, rotate, ROTATE, animation);*/

			XMMATRIX keyValue = GetKeyValue(past, *animation);
			XMMATRIX value = XMMatrixMultiply(XMMatrixInverse(nullptr, target->skeleton->bones[node.boneIdx].offsetMat),keyValue);

			if (affectMode == ASSIGN)
			{
				target->boneMatrices[node.boneIdx] = value;
			}
			else if (affectMode == ADDITIONAL)
			{
				target->boneMatrices[node.boneIdx] *= value;
			}
		}
		past++;	//フレーム経過

		if (/*t == 1.0f*/past == total - 1)
		{
			if (loop)
			{
				past = 0;
			}
			else
			{
				isPlay = false;
				target->nowAnimation--;
				target = nullptr;
			}
		}
	}
}

void Action::Pose(int KeyFrame, HSM::BoneGroup* BoneGroup)
{
	if (!isPlay)
	{
		poseKeyFrame = KeyFrame;
		poseTarget = BoneGroup;
		this->Update();
	}
}