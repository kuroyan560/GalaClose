#pragma once
#include"MyFunc.h"

struct BoneNode
{
	int boneIdx;	//ボーンインデックス
	MyDef::float3 startPos;	//ボーン基準点(回転の中心)
	MyDef::float3 endPos;	//ボーン先端点(実際のスキニングには利用しない)
	std::vector<BoneNode*>children;	//子ノード
};