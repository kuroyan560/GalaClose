#include "PmxLoader.h"
#include"WinApp.h"
#include<iterator>
#include"SRVDescMgr.h"

using namespace PMX;

void LoadTextBuf_int(FILE* Fp, string* StrBuff)
{
	unsigned int length;	//バイト長
	fread(&length, sizeof(length), 1, Fp);

	vector<char>str;
	str.resize(length);	//文字列のサイズ変更
	fread(str.data(), sizeof(str[0]), length, Fp);

	for (auto itr : str)
	{
		StrBuff->push_back(itr);
	}
}

void LoadPMXHeader(FILE* Fp, PMXHeader* Tmp)
{
	fread(&Tmp->format, sizeof(Tmp->format), 1, Fp);	//"PMX "
	fread(&Tmp->version, sizeof(Tmp->version), 1, Fp);	//ver(2.0/2.1)
	unsigned char byteSize;	//後続するデータ列のバイトサイズ
	fread(&byteSize, sizeof(byteSize), 1, Fp);
	Tmp->byteArray.resize(byteSize);	//サイズ変更
	fread(Tmp->byteArray.data(), sizeof(Tmp->byteArray[0]), byteSize, Fp);
}

void LoadPMXInfo(FILE* Fp, PMXInfo* Tmp)
{
	LoadTextBuf_int(Fp, &Tmp->modelName_JP);	//モデル名
	LoadTextBuf_int(Fp, &Tmp->modelName_EN);	//モデル名英
	LoadTextBuf_int(Fp, &Tmp->comment_JP);	//コメント
	LoadTextBuf_int(Fp, &Tmp->comment_EN);	//コメント英
}

void LoadPMXVertex(FILE* Fp, PMXVertex* Tmp, int UVAddSize, size_t BoneIndexSize)
{
	fread(&Tmp->pos, sizeof(Tmp->pos), 1, Fp);	//位置
	fread(&Tmp->normal, sizeof(Tmp->normal), 1, Fp);	//法線
	fread(&Tmp->uv, sizeof(Tmp->uv), 1, Fp);	//法線

	//追加UV
	Tmp->additionalUv.resize(UVAddSize);
	fread(Tmp->additionalUv.data(), sizeof(Tmp->additionalUv[0]), UVAddSize, Fp);

	//ウェイト
	fread(&Tmp->weightTransMethod, sizeof(Tmp->weightTransMethod), 1, Fp);	//ウェイト変形方式
	if (Tmp->weightTransMethod == BDEF1)
	{
		fread(Tmp->boneIndex[0], BoneIndexSize, 1, Fp);	//１本のボーン番号のみ
	}
	else if (Tmp->weightTransMethod == BDEF2 || Tmp->weightTransMethod == SDEF)	//PMD方式
	{
		for (int i = 0; i < 2; i++)		//２本のボーン番号
		{
			fread(Tmp->boneIndex[i], BoneIndexSize, 1, Fp);	
		}
		fread(&Tmp->boneWeight[0], sizeof(Tmp->boneWeight[0]), 1, Fp);	//１本のボーンのウェイト値
	}
	else if (Tmp->weightTransMethod == BDEF4)
	{
		for (int i = 0; i < 4; i++)		//４本のボーン番号
		{
			fread(Tmp->boneIndex[i], BoneIndexSize, 1, Fp);	
			fread(&Tmp->boneWeight[i], sizeof(Tmp->boneWeight[0]), 1, Fp);	//４本のそれぞれのウェイト（合計が1.0である保証はしない）
		}
	}

	if (Tmp->weightTransMethod == SDEF)	//BDEFに加えて、SDEF用のfloat3 * 3
	{
		fread(&Tmp->SDEF_C, sizeof(Tmp->SDEF_C), 1, Fp);
		fread(&Tmp->SDEF_R0, sizeof(Tmp->SDEF_R0), 1, Fp);
		fread(&Tmp->SDEF_R1, sizeof(Tmp->SDEF_R1), 1, Fp);
	}

	//エッジ倍率
	fread(&Tmp->edgeRate, sizeof(Tmp->edgeRate), 1, Fp);
}

void LoadPMXMaterial(FILE* Fp, PMXMaterial* Tmp, size_t TexIndexSize)
{
	LoadTextBuf_int(Fp, &Tmp->materialName_JP);
	LoadTextBuf_int(Fp, &Tmp->materialName_EN);
	fread(&Tmp->diffuse, sizeof(Tmp->diffuse), 1, Fp);
	fread(&Tmp->specular, sizeof(Tmp->specular), 1, Fp);
	fread(&Tmp->specular_c, sizeof(Tmp->specular_c), 1, Fp);
	fread(&Tmp->ambient, sizeof(Tmp->ambient), 1, Fp);
	fread(&Tmp->bitFlag, sizeof(Tmp->bitFlag), 1, Fp);
	fread(&Tmp->edgeColor, sizeof(Tmp->edgeColor), 1, Fp);
	fread(&Tmp->edgeSize, sizeof(Tmp->edgeSize), 1, Fp);

	fread(Tmp->normalTexIndex, TexIndexSize, 1, Fp);

	fread(Tmp->sphereTexIndex, TexIndexSize, 1, Fp);

	fread(&Tmp->sphereMode, sizeof(Tmp->sphereMode), 1, Fp);
	fread(&Tmp->shareToonFlag, sizeof(Tmp->shareToonFlag), 1, Fp);

	if (Tmp->shareToonFlag == 0)
	{
		fread(Tmp->toonTexIndex, TexIndexSize, 1, Fp);
	}
	else if (Tmp->shareToonFlag == 1)
	{
		fread(Tmp->toonTex, TexIndexSize, 1, Fp);
	}

	LoadTextBuf_int(Fp, &Tmp->memo);

	fread(&Tmp->forSurfaceNum, sizeof(Tmp->forSurfaceNum), 1, Fp);
}

void LoadPMXBone(FILE* Fp, PMXBone* Tmp,size_t BoneIndexSize)
{
	string tmp;
	LoadTextBuf_int(Fp, &tmp);

	auto* wstrBytes = new wchar_t[tmp.size()];
	memcpy_s(wstrBytes, tmp.size(), tmp.c_str(), tmp.size());
	std::wstring unicodeStr(wstrBytes, tmp.size());
	delete[] wstrBytes;

	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, unicodeStr.c_str(), -1, (char*)NULL, 0, NULL, NULL);
	char* cpMultiByte = new char[iBufferSize];
	WideCharToMultiByte(CP_OEMCP, 0, unicodeStr.c_str(), -1, cpMultiByte, iBufferSize, NULL, NULL);
	std::string oRet(cpMultiByte, tmp.size());
	delete[] cpMultiByte;

	Tmp->name_JP = oRet;

	LoadTextBuf_int(Fp, &Tmp->name_EN);
	fread(&Tmp->pos, sizeof(Tmp->pos), 1, Fp);

	fread(Tmp->boneIndex, BoneIndexSize, 1, Fp);

	fread(&Tmp->transLayer, sizeof(Tmp->transLayer), 1, Fp);
	fread(&Tmp->boneFlag, sizeof(Tmp->boneFlag), 1, Fp);

	//接続先(PMD子ボーン指定)表示方法 -> 0:座標オフセットで指定 1:ボーンで指定
	if ((0x0001 & Tmp->boneFlag) == 0)
	{
		Bone::PosOffset_0x0001 offset;
		fread(&offset.posOffset, sizeof(offset.posOffset), 1, Fp);
		Tmp->info.push_back(new Bone::PosOffset_0x0001(offset));
	}
	else
	{
		Bone::Index_0x0001 index(BoneIndexSize);
		fread(index.boneIndex, BoneIndexSize, 1, Fp);
		Tmp->info.push_back(new Bone::Index_0x0001(std::move(index)));
	}
	if ((0x0100 & Tmp->boneFlag) != 0 || (0x0200 & Tmp->boneFlag) != 0)
	{
		Bone::Grant grant(BoneIndexSize);
		fread(grant.boneIndex, BoneIndexSize, 1, Fp);
		fread(&grant.rate, sizeof(grant.rate), 1, Fp);
		Tmp->info.push_back(new Bone::Grant(std::move(grant)));
	}
	//軸固定:1 の場合
	if ((0x0400 & Tmp->boneFlag) != 0)
	{
		Bone::AxisVec vec;
		fread(&vec.vec, sizeof(vec.vec), 1, Fp);
		Tmp->info.push_back(new Bone::AxisVec(vec));
	}
	//ローカル軸:1 の場合
	if ((0x0800 & Tmp->boneFlag) != 0)
	{
		Bone::LocalAxisVec vec;
		fread(&vec.xVec, sizeof(vec.xVec), 1, Fp);
		fread(&vec.zVec, sizeof(vec.zVec), 1, Fp);
		Tmp->info.push_back(new Bone::LocalAxisVec(vec));
	}
	//外部親変形:1 の場合
	if ((0x2000 & Tmp->boneFlag) != 0)
	{
		Bone::Key key;
		fread(&key.key, sizeof(key.key), 1, Fp);
		Tmp->info.push_back(new Bone::Key(key));
	}
	//IK:1 の場合 IKデータを格納
	if ((0x0020 & Tmp->boneFlag) != 0)
	{
		Bone::IK ik(BoneIndexSize);
		fread(ik.boneIndex, BoneIndexSize, 1, Fp);
		fread(&ik.loopTimes, sizeof(ik.loopTimes), 1, Fp);
		fread(&ik.limitAngle, sizeof(ik.limitAngle), 1, Fp);
		fread(&ik.linkNum, sizeof(ik.linkNum), 1, Fp);

		for (int i = 0; i < ik.linkNum; i++)
		{
			ik.IKlinks.push_back(BoneIndexSize);
			fread(ik.IKlinks.back().boneIndex, BoneIndexSize, 1, Fp);
			fread(&ik.IKlinks.back().limitAngleFlag, sizeof(ik.IKlinks.back().limitAngleFlag), 1, Fp);
			if (ik.IKlinks.back().limitAngleFlag)
			{
				fread(&ik.IKlinks.back().upperLimit, sizeof(ik.IKlinks.back().upperLimit), 1, Fp);
				fread(&ik.IKlinks.back().lowerLimit, sizeof(ik.IKlinks.back().lowerLimit), 1, Fp);
			}
			//ik.IKlinks.push_back(new Bone::IK::IKLink(link));
		}
		Tmp->info.push_back(new Bone::IK(std::move(ik)));
		//Tmp->info.emplace_back(ik);
	}
}

void LoadPMXMorph(FILE* Fp, PMXMorph* Tmp,
	size_t VertexIndexSize, size_t BoneIndexSize, size_t MaterialIndexSize, size_t MorphIndexSize)
{
	LoadTextBuf_int(Fp, &Tmp->name_JP);
	LoadTextBuf_int(Fp, &Tmp->name_EN);
	fread(&Tmp->controlPanel, sizeof(Tmp->controlPanel), 1, Fp);
	fread(&Tmp->type, sizeof(Tmp->type), 1, Fp);
	fread(&Tmp->offsetNum, sizeof(Tmp->offsetNum), 1, Fp);

	for (int i = 0; i < Tmp->offsetNum; i++)
	{
		if (Tmp->type == PMXMorph::VERTEX)
		{
			Morph::Vertex vertex(VertexIndexSize);
			fread(vertex.vertexIndex, VertexIndexSize, 1, Fp);
			fread(&vertex.offset, sizeof(vertex.offset), 1, Fp);
			Tmp->data.emplace_back(std::move(vertex));
		}
		else if (Tmp->type == PMXMorph::UV)
		{
			Morph::UV uv(VertexIndexSize);
			fread(&uv.vertexIndex, VertexIndexSize, 1, Fp);
			fread(&uv.offset, sizeof(uv.offset), 1, Fp);
			Tmp->data.emplace_back(std::move(uv));
		}
		else if (Tmp->type == PMXMorph::BONE)
		{
			Morph::Bone bone(BoneIndexSize);
			fread(&bone.boneIndex, BoneIndexSize, 1, Fp);
			fread(&bone.move, sizeof(bone.move), 1, Fp);
			fread(&bone.quaternion, sizeof(bone.quaternion), 1, Fp);
			Tmp->data.emplace_back(std::move(bone));
		}
		else if (Tmp->type == PMXMorph::MATERIAL)
		{
			Morph::Material material(MaterialIndexSize);
			fread(&material.materialIndex, MaterialIndexSize, 1, Fp);
			fread(&material.offsetFormat, sizeof(material.offsetFormat), 1, Fp);
			fread(&material.duffse, sizeof(material.duffse), 1, Fp);
			fread(&material.specular, sizeof(material.specular), 1, Fp);
			fread(&material.specular_c, sizeof(material.specular_c), 1, Fp);
			fread(&material.ambient, sizeof(material.ambient), 1, Fp);
			fread(&material.edgeColor, sizeof(material.edgeColor), 1, Fp);
			fread(&material.edgeSize, sizeof(material.edgeSize), 1, Fp);
			fread(&material.tex_c, sizeof(material.tex_c), 1, Fp);
			fread(&material.sphere_c, sizeof(material.sphere_c), 1, Fp);
			fread(&material.toonTex_c, sizeof(material.toonTex_c), 1, Fp);
			Tmp->data.emplace_back(std::move(material));
		}
		else if (Tmp->type == PMXMorph::GROUP)
		{
			Morph::Group group(MorphIndexSize);
			fread(&group.morphIndex, MorphIndexSize, 1, Fp);
			fread(&group.rate, sizeof(group.rate), 1, Fp);
			Tmp->data.emplace_back(std::move(group));
		}
	}
}

void IgnoreDisplays(FILE* Fp, size_t BoneIndexSize, size_t MorphIndexSize)
{
	int displayNum;
	fread(&displayNum, sizeof(displayNum), 1, Fp);

	for (int k = 0; k < displayNum; k++)
	{
		string name_JP;
		LoadTextBuf_int(Fp, &name_JP);
		string name_EN;
		LoadTextBuf_int(Fp, &name_EN);

		unsigned char specialFlag; // 特殊枠フラグ - 0:通常枠 1:特殊枠
		fread(&specialFlag, sizeof(specialFlag), 1, Fp);

		int num; //枠内要素数 : 後続の要素数
		fread(&num, sizeof(num), 1, Fp);

		for (int i = 0; i < num; i++)
		{
			unsigned char target;
			fread(&target, sizeof(target), 1, Fp);

			if (target == 0)
			{
				unsigned char* boneIndex;
				boneIndex = new unsigned char[BoneIndexSize];
				fread(boneIndex, BoneIndexSize, 1, Fp);
				delete[] boneIndex;
			}
			else if (target == 1)
			{
				unsigned char* morphIndex;
				morphIndex = new unsigned char[MorphIndexSize];
				fread(morphIndex, MorphIndexSize, 1, Fp);
				delete[] morphIndex;
			}
		}
	}
}

void LoadPMXRigidBody(FILE* Fp, PMXRigidBody* Tmp, size_t BoneIndexSize)
{
	LoadTextBuf_int(Fp, &Tmp->name_JP);
	LoadTextBuf_int(Fp, &Tmp->name_EN);

	fread(Tmp->boneIndex, BoneIndexSize, 1, Fp);

	fread(&Tmp->group, sizeof(Tmp->group), 1, Fp);
	fread(&Tmp->notCollisionGroupFlag, sizeof(Tmp->notCollisionGroupFlag), 1, Fp);
	fread(&Tmp->shape, sizeof(Tmp->shape), 1, Fp);
	fread(&Tmp->size, sizeof(Tmp->size), 1, Fp);
	fread(&Tmp->pos, sizeof(Tmp->pos), 1, Fp);
	fread(&Tmp->rotate, sizeof(Tmp->rotate), 1, Fp);
	fread(&Tmp->mass, sizeof(Tmp->mass), 1, Fp);
	fread(&Tmp->moveAttenuation, sizeof(Tmp->moveAttenuation), 1, Fp);
	fread(&Tmp->rotateAttenuation, sizeof(Tmp->rotateAttenuation), 1, Fp);
	fread(&Tmp->rebound, sizeof(Tmp->rebound), 1, Fp);
	fread(&Tmp->friction, sizeof(Tmp->friction), 1, Fp);
	fread(&Tmp->physicalOperation, sizeof(Tmp->physicalOperation), 1, Fp);
}

void LoadPMXJoint(FILE* Fp, PMXJoint* Tmp, size_t RigidBodyIndexSize)
{
	LoadTextBuf_int(Fp, &Tmp->name_JP);
	LoadTextBuf_int(Fp, &Tmp->name_EN);
	fread(&Tmp->type, sizeof(Tmp->type), 1, Fp);

	if (Tmp->type == 0)
	{
		Tmp->info = new PMXJoint::JointInfo(RigidBodyIndexSize);

		fread(Tmp->info->rigidBodyIndexA, RigidBodyIndexSize, 1, Fp);

		fread(Tmp->info->rigidBodyIndexB, RigidBodyIndexSize, 1, Fp);

		fread(&Tmp->info->pos, sizeof(Tmp->info->pos), 1, Fp);
		fread(&Tmp->info->spin, sizeof(Tmp->info->spin), 1, Fp);
		fread(&Tmp->info->moveLimit_Lower, sizeof(Tmp->info->moveLimit_Lower), 1, Fp);
		fread(&Tmp->info->moveLimit_Upper, sizeof(Tmp->info->moveLimit_Upper), 1, Fp);
		fread(&Tmp->info->spinLimit_Lower, sizeof(Tmp->info->spinLimit_Lower), 1, Fp);
		fread(&Tmp->info->spinLimit_Upper, sizeof(Tmp->info->spinLimit_Upper), 1, Fp);
		fread(&Tmp->info->springConstant_Move, sizeof(Tmp->info->springConstant_Move), 1, Fp);
		fread(&Tmp->info->springConstant_Spin, sizeof(Tmp->info->springConstant_Spin), 1, Fp);
	}
}

void PMXLoader::LoadPMX(string Dir, string ModelName, Model_HSM** Model)
{
	if (*Model != nullptr)
	{
		delete *Model;
		*Model = nullptr;
	}

#pragma region PMXファイルオープン
	FILE* fp;
	fopen_s(&fp, (Dir + ModelName).c_str(), "rb");
	if (fp == NULL)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "PMXファイルのオープンに失敗");
		DebugBreak();
	}
#pragma endregion

	PMXModel pmx;
	LoadPMXHeader(fp, &pmx.head); //ヘッダ読み込み
	LoadPMXInfo(fp, &pmx.info);	//モデル情報読み込み

#pragma region インデックス番号サイズ
	size_t size[6] = { 0 };
	for (int i = 0; i < HSM::TYPE_SIZE_NUM; i++)
	{
		size[i] = pmx.head.byteArray[i + PMX_BYTE_ARRAY_INDEX_SIZE_OFFSET];
	}
#pragma endregion

#pragma region 頂点データ
	fread(&pmx.vertexNum, sizeof(pmx.vertexNum), 1, fp);
	for (int i = 0; i < pmx.vertexNum; i++)
	{
		PMXVertex vertex(size[HSM::INDEX_BYTE_SIZE::BONE_SIZE]);
		LoadPMXVertex(fp, &vertex, pmx.head.byteArray[1], size[HSM::INDEX_BYTE_SIZE::BONE_SIZE]);
		pmx.vertices.push_back(std::move(vertex));
	}
#pragma endregion

#pragma region 面
	fread(&pmx.surfaceNum, sizeof(pmx.surfaceNum), 1, fp);
	for (int i = 0; i < pmx.surfaceNum / 3; i++)
	{
		PMXSurface surface(size[HSM::INDEX_BYTE_SIZE::VERTEX_SIZE]);
		for (int num = 0; num < 3; num++)
		{
			fread(surface.vertIndex[num], size[HSM::INDEX_BYTE_SIZE::VERTEX_SIZE], 1, fp);
		}
		pmx.surfaces.push_back(std::move(surface));
	}
#pragma endregion

#pragma region テクスチャ
	fread(&pmx.texNum, sizeof(pmx.texNum), 1, fp);
	for (int i = 0; i < pmx.texNum; i++)
	{
		string str;
		LoadTextBuf_int(fp, &str);
		pmx.texPass.push_back(str);
	}
#pragma endregion

#pragma region マテリアル
	fread(&pmx.materialNum, sizeof(pmx.materialNum), 1, fp);
	for (int i = 0; i < pmx.materialNum; i++)
	{
		PMXMaterial material(size[HSM::INDEX_BYTE_SIZE::TEXTURE_SIZE]);
		LoadPMXMaterial(fp, &material, size[HSM::INDEX_BYTE_SIZE::TEXTURE_SIZE]);
		pmx.materials.push_back(std::move(material));
	}
#pragma endregion

#pragma region ボーン
	fread(&pmx.boneNum, sizeof(pmx.boneNum), 1, fp);
	for (int i = 0; i < pmx.boneNum; i++)
	{
		pmx.bones.push_back(PMXBone(size[HSM::INDEX_BYTE_SIZE::BONE_SIZE]));
		LoadPMXBone(fp, &pmx.bones.back(), size[HSM::INDEX_BYTE_SIZE::BONE_SIZE]);
	}
#pragma endregion

#pragma region モーフ
	fread(&pmx.morphNum, sizeof(pmx.morphNum), 1, fp);
	for (int i = 0; i < pmx.morphNum; i++)
	{
		pmx.morphs.emplace_back();
		LoadPMXMorph(fp, &pmx.morphs.back(),
			size[HSM::INDEX_BYTE_SIZE::VERTEX_SIZE], size[HSM::INDEX_BYTE_SIZE::BONE_SIZE],
			size[HSM::INDEX_BYTE_SIZE::MATERIAL_SIZE], size[HSM::INDEX_BYTE_SIZE::MORPH_SIZE]);
	}
#pragma endregion

	//表示枠の情報は必要ないのでスルー
	IgnoreDisplays(fp, size[HSM::INDEX_BYTE_SIZE::BONE_SIZE], size[HSM::INDEX_BYTE_SIZE::MORPH_SIZE]);

#pragma region 剛体
	fread(&pmx.rigidBodyNum, sizeof(pmx.rigidBodyNum), 1, fp);
	for (int i = 0; i < pmx.rigidBodyNum; i++)
	{
		pmx.rigitBodies.push_back(PMXRigidBody(size[HSM::INDEX_BYTE_SIZE::BONE_SIZE]));
		LoadPMXRigidBody(fp, &pmx.rigitBodies[i], size[HSM::INDEX_BYTE_SIZE::BONE_SIZE]);
	}
#pragma endregion

#pragma region ジョイント
	fread(&pmx.jointNum, sizeof(pmx.jointNum), 1, fp);
	for (int i = 0; i < pmx.jointNum; i++)
	{
		pmx.joints.emplace_back();
		LoadPMXJoint(fp, &pmx.joints.back(), size[HSM::INDEX_BYTE_SIZE::RIGIDBODY_SIZE]);
	}
#pragma endregion

	if (fclose(fp) == EOF)
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "PMXファイルのクローズに失敗");
	}

#pragma region HSMにコンバート
	*Model = new Model_HSM();
	ConvertPMX(Dir, &pmx, *Model);
	(*Model)->CreateBoneTree();
	(*Model)->CreateBuff();
#pragma endregion
}

void PMXLoader::ConvertPMX(string Dir, PMXModel* Pmx, Model_HSM* Hsm)
{
	Hsm->head.name = Pmx->info.modelName_JP;
	for (int i = 0; i < HSM::INDEX_BYTE_SIZE::TYPE_SIZE_NUM; i++)
	{
		Hsm->head.byteSize[i] = Pmx->head.byteArray[i + PMX_BYTE_ARRAY_INDEX_SIZE_OFFSET];
	}

#pragma region 頂点
	size_t boneIndexSize = Hsm->head.byteSize[HSM::INDEX_BYTE_SIZE::BONE_SIZE];
	Hsm->vertices.resize(Pmx->vertexNum);
	for (int i = 0; i < Pmx->vertexNum; i++)
	{
		Hsm->vertices[i].pos = Pmx->vertices[i].pos;
		Hsm->vertices[i].normal = Pmx->vertices[i].normal;
		Hsm->vertices[i].uv = Pmx->vertices[i].uv;
		for (int j = 0; j < 4; ++j)
		{
			if (boneIndexSize == 1)
			{
				Hsm->vertices[i].boneIndex[j] = *(unsigned char*)Pmx->vertices[i].boneIndex[j];
			}
			else if (boneIndexSize == 2)
			{
				Hsm->vertices[i].boneIndex[j] = *(unsigned short*)Pmx->vertices[i].boneIndex[j];
			}
			else if (boneIndexSize == 4)
			{
				Hsm->vertices[i].boneIndex[j] = *(unsigned int*)Pmx->vertices[i].boneIndex[j];
			}
			//無効な値なら-1
			if (Hsm->vertices[i].boneIndex[j] < 0 || Pmx->boneNum <= Hsm->vertices[i].boneIndex[j])
			{
				Hsm->vertices[i].boneIndex[j] = -1;
			}

			Hsm->vertices[i].boneWeight[j] = Pmx->vertices[i].boneWeight[j];
		}
	}
#pragma endregion

#pragma region インデックス
	size_t vertIndexSize = Hsm->head.byteSize[HSM::INDEX_BYTE_SIZE::VERTEX_SIZE];
	for (int i = 0; i < Pmx->surfaces.size(); i++)
	{
		for (int num = 0; num < 3; num++)
		{
			if (vertIndexSize == 1)
			{
				unsigned char hoge = *(unsigned char*)Pmx->surfaces[i].vertIndex[num];
				Hsm->indices.emplace_back(hoge);
			}
			else if (vertIndexSize == 2)
			{
				unsigned short hoge = *(unsigned short*)Pmx->surfaces[i].vertIndex[num];
				Hsm->indices.emplace_back(hoge);
			}
			else if (vertIndexSize == 4)
			{
				unsigned int hoge = *(unsigned int*)Pmx->surfaces[i].vertIndex[num];
				Hsm->indices.emplace_back(hoge);
			}
		}
	}
#pragma endregion

#pragma region テクスチャとマテリアル
	vector<int>texArray;
	for (int i = 0; i < Pmx->texNum; i++)
	{
		//テクスチャファイル名に\0が入っている場合は、読み込み時にファイルを見つけられなくなるので消す
		for (size_t c = Pmx->texPass[i].find_first_of('\0'); c != string::npos; c = c = Pmx->texPass[i].find_first_of('\0')) {
			Pmx->texPass[i].erase(c, 1);
		}
		texArray.push_back(SRVDescMgr::Instance()->CreateSRV(Dir + Pmx->texPass[i]));
	}

	//size_t texIndexSize = Pmx->head.byteArray[HSM::BYTE_ARRAY_INDEX_SIZE::TEXTURE_SIZE + PMX_BYTE_ARRAY_INDEX_SIZE_OFFSET];
	size_t texIndexSize = Hsm->head.byteSize[HSM::INDEX_BYTE_SIZE::TEXTURE_SIZE];
	for (int i = 0; i < Pmx->materialNum; i++)
	{
		Hsm->materials.emplace_back();

		Material_HSMA* hsma = &Hsm->materials[i];
		PMX::PMXMaterial* pmxMat = &Pmx->materials[i];
		hsma->head.name = pmxMat->materialName_JP;

		HSM::Mesh mesh;
		mesh.materialIndex = i;
		mesh.name = hsma->head.name;
		mesh.vertexNum = Pmx->materials[i].forSurfaceNum;
		Hsm->meshes.emplace_back(mesh);

		hsma->material.material.diffuse = pmxMat->diffuse;
		hsma->material.material.specular = pmxMat->specular;
		hsma->material.material.specular_c = pmxMat->specular_c;
		hsma->material.material.ambient = pmxMat->ambient;
		hsma->material.additional.bitFlag = pmxMat->bitFlag;
		hsma->material.additional.edgeColor = pmxMat->edgeColor;
		hsma->material.additional.edgeSize = pmxMat->edgeSize;
		hsma->material.additional.sphereMode = pmxMat->sphereMode;

		if (texIndexSize == 1)
		{
			if (pmxMat->normalTexIndex != nullptr && (unsigned char)*pmxMat->normalTexIndex < Pmx->texNum)
			{
				hsma->normalTexHandle = texArray[(unsigned char)*pmxMat->normalTexIndex];
			}
			if (pmxMat->sphereTexIndex != nullptr && (unsigned char)*pmxMat->sphereTexIndex < Pmx->texNum)
			{
				hsma->sphereTexHandle = texArray[(unsigned char)*pmxMat->sphereTexIndex];
			}
			if (pmxMat->toonTexIndex != nullptr && (unsigned char)*pmxMat->toonTexIndex < Pmx->texNum)
			{
				hsma->toonTexHandle = texArray[(unsigned char)*pmxMat->toonTexIndex];
			}
		}
		else if (texIndexSize == 2)
		{
			if (pmxMat->normalTexIndex != nullptr && (unsigned short)*pmxMat->normalTexIndex < Pmx->texNum)
			{
				hsma->normalTexHandle = texArray[(unsigned short)*pmxMat->normalTexIndex];
			}
			if (pmxMat->sphereTexIndex != nullptr && (unsigned short)*pmxMat->sphereTexIndex < Pmx->texNum)
			{
				hsma->sphereTexHandle = texArray[(unsigned short)*pmxMat->sphereTexIndex];
			}
			if (pmxMat->toonTexIndex != nullptr && (unsigned short)*pmxMat->toonTexIndex < Pmx->texNum)
			{
				hsma->toonTexHandle = texArray[(unsigned short)*pmxMat->toonTexIndex];
			}
		}
		else if (texIndexSize == 4)
		{
			if (pmxMat->normalTexIndex != nullptr && (unsigned int)*pmxMat->normalTexIndex < Pmx->texNum)
			{
				hsma->normalTexHandle = texArray[(unsigned int)*pmxMat->normalTexIndex];
			}
			if (pmxMat->sphereTexIndex != nullptr && (unsigned int)*pmxMat->sphereTexIndex < Pmx->texNum)
			{
				hsma->sphereTexHandle = texArray[(unsigned int)*pmxMat->sphereTexIndex];
			}
			if (pmxMat->toonTexIndex != nullptr && (unsigned int)*pmxMat->toonTexIndex < Pmx->texNum)
			{
				hsma->toonTexHandle = texArray[(unsigned int)*pmxMat->toonTexIndex];
			}
		}
	}
#pragma endregion

#pragma region ボーン
	auto itr = Pmx->bones.begin();
	for (int i = 0; i < Pmx->boneNum; ++i)
	{
		Hsm->skeleton.bones.emplace_back();

		Hsm->skeleton.bones[i].name = itr->name_JP;
		Hsm->skeleton.bones[i].parent = *(itr->boneIndex);
		Hsm->skeleton.bones[i].pos = itr->pos;
		Hsm->skeleton.bones[i].transLayer = itr->transLayer;

		++itr;
	}
#pragma endregion
}

PMX::PMXSurface::PMXSurface(size_t Size)
{
	for (int i = 0; i < 3; i++)
	{
		vertIndex[i] = new unsigned char[Size];
	}
}

PMX::PMXSurface::~PMXSurface()
{
	for (int i = 0; i < 3; i++)
	{
		delete[] vertIndex[i];
		vertIndex[i] = nullptr;
	}
}

PMX::PMXSurface::PMXSurface(PMXSurface&& Tmp)
{
	for (int i = 0; i < 3; i++)
	{
		vertIndex[i] = Tmp.vertIndex[i];
		Tmp.vertIndex[i] = nullptr;
	}
}

PMX::PMXVertex::PMXVertex(size_t BoneIndexSize)
{
	for (int i = 0; i < 4; i++)
	{
		boneIndex[i] = new unsigned char[BoneIndexSize];
	}
}

PMX::PMXVertex::~PMXVertex()
{
	for (int i = 0; i < 4; i++)
	{
		delete[] boneIndex[i];
		boneIndex[i] = nullptr;
	}
}

PMX::PMXVertex::PMXVertex(PMXVertex&& Tmp)
	:pos(Tmp.pos), normal(Tmp.normal), uv(Tmp.uv),
	additionalUv(Tmp.additionalUv), weightTransMethod(Tmp.weightTransMethod),
	SDEF_C(Tmp.SDEF_C), SDEF_R0(Tmp.SDEF_R0), SDEF_R1(Tmp.SDEF_R1)
{
	for (int i = 0; i < 4; i++)
	{
		this->boneWeight[i] = Tmp.boneWeight[i];
		this->boneIndex[i] = Tmp.boneIndex[i];
		Tmp.boneIndex[i] = nullptr;
	}
}

PMX::PMXMaterial::PMXMaterial(size_t TexIndexSize)
{
	normalTexIndex = new unsigned char[TexIndexSize];
	sphereTexIndex = new unsigned char[TexIndexSize];
	toonTexIndex = new unsigned char[TexIndexSize];
	toonTex = new unsigned char[TexIndexSize];
}

PMX::PMXMaterial::~PMXMaterial()
{
	delete[] normalTexIndex;
	normalTexIndex = nullptr;
	delete[] sphereTexIndex;
	sphereTexIndex = nullptr;
	delete[] toonTexIndex;
	toonTexIndex = nullptr;
	delete[] toonTex;
	toonTex = nullptr;
}

PMX::PMXMaterial::PMXMaterial(PMXMaterial&& Tmp)
	:materialName_JP(Tmp.materialName_JP), materialName_EN(Tmp.materialName_EN),
	diffuse(Tmp.diffuse), specular(Tmp.specular), specular_c(Tmp.specular_c),
	ambient(Tmp.ambient), bitFlag(Tmp.bitFlag), edgeColor(Tmp.edgeColor), edgeSize(Tmp.edgeSize),
	sphereMode(Tmp.sphereMode), shareToonFlag(Tmp.shareToonFlag), memo(Tmp.memo), forSurfaceNum(Tmp.forSurfaceNum)
{
	normalTexIndex = Tmp.normalTexIndex;
	Tmp.normalTexIndex = nullptr;
	sphereTexIndex = Tmp.sphereTexIndex;
	Tmp.sphereTexIndex = nullptr;
	toonTexIndex = Tmp.toonTexIndex;
	Tmp.toonTexIndex = nullptr;
	toonTex = Tmp.toonTex;
	Tmp.toonTex = nullptr;
}

PMX::Bone::Index_0x0001::Index_0x0001(size_t BoneIndexSize)
{
	boneIndex = new unsigned char[BoneIndexSize];
}

PMX::Bone::Index_0x0001::~Index_0x0001()
{
	delete[] boneIndex;
	boneIndex = nullptr;
}

PMX::Bone::Index_0x0001::Index_0x0001(Index_0x0001&& Tmp)
{
	this->boneIndex = Tmp.boneIndex;
	Tmp.boneIndex = nullptr;
}

PMX::Bone::Grant::Grant(size_t BoneIndexSize)
{
	boneIndex = new unsigned char[BoneIndexSize];
}

PMX::Bone::Grant::~Grant()
{
	delete[] boneIndex;
	boneIndex = nullptr;
}

PMX::Bone::Grant::Grant(Grant&& Tmp)
	:rate(Tmp.rate), boneIndex(Tmp.boneIndex)
{
	Tmp.boneIndex = nullptr;
}

PMX::Bone::IK::IK(size_t BoneIndexSize)
{
	boneIndex = new unsigned char[BoneIndexSize];
}

PMX::Bone::IK::~IK()
{
	delete[] boneIndex;
	boneIndex = nullptr;
}

PMX::Bone::IK::IK(IK&& Tmp)
	:boneIndex(Tmp.boneIndex),
	loopTimes(Tmp.loopTimes), limitAngle(Tmp.limitAngle), linkNum(Tmp.linkNum)
{
	std::move(Tmp.IKlinks.begin(), Tmp.IKlinks.end(), std::back_inserter(IKlinks));
	Tmp.boneIndex = nullptr;
}

PMX::Bone::IK::IKLink::IKLink(size_t BoneIndexSize)
{
	boneIndex = new unsigned char[BoneIndexSize];
}

PMX::Bone::IK::IKLink::~IKLink()
{
	delete[] boneIndex;
	boneIndex = nullptr;
}

PMX::Bone::IK::IKLink::IKLink(IKLink&& Tmp)
	:boneIndex(Tmp.boneIndex), limitAngleFlag(Tmp.limitAngleFlag),
	lowerLimit(Tmp.lowerLimit), upperLimit(Tmp.upperLimit)
{
	Tmp.boneIndex = nullptr;
}

PMX::PMXBone::PMXBone(size_t BoneIndexSize)
{
	boneIndex = new unsigned char[BoneIndexSize];
}

PMX::PMXBone::~PMXBone()
{
	delete[] boneIndex;
	boneIndex = nullptr;

	for (auto itr : info)
	{
		delete itr;
	}
}

PMX::PMXBone::PMXBone(PMXBone&& Tmp)
	:name_JP(Tmp.name_JP), name_EN(Tmp.name_EN),
	pos(Tmp.pos), boneIndex(Tmp.boneIndex),
	transLayer(Tmp.transLayer), boneFlag(Tmp.boneFlag),
	info(Tmp.info)
{
	Tmp.boneIndex = nullptr;
}

PMX::Morph::Vertex::Vertex(size_t VertexIndexSize)
{
	vertexIndex = new unsigned char[VertexIndexSize];
}

PMX::Morph::Vertex::~Vertex()
{
	delete[] vertexIndex;
	vertexIndex = nullptr;
}

PMX::Morph::Vertex::Vertex(Vertex&& Tmp)
	:vertexIndex(Tmp.vertexIndex), offset(Tmp.offset)
{
	Tmp.vertexIndex = nullptr;
}

PMX::Morph::UV::UV(size_t VertexIndexSize)
{
	vertexIndex = new unsigned char[VertexIndexSize];
}

PMX::Morph::UV::~UV()
{
	delete[] vertexIndex;
	vertexIndex = nullptr;
}

PMX::Morph::UV::UV(UV&& Tmp)
	:vertexIndex(Tmp.vertexIndex), offset(Tmp.offset)
{
	Tmp.vertexIndex = nullptr;
}

PMX::Morph::Bone::Bone(size_t BoneIndexSize)
{
	boneIndex = new unsigned char[BoneIndexSize];
}

PMX::Morph::Bone::~Bone()
{
	delete[] boneIndex;
	boneIndex = nullptr;
}

PMX::Morph::Bone::Bone(Bone&& Tmp)
	:boneIndex(Tmp.boneIndex), move(Tmp.move), quaternion(Tmp.quaternion)
{
	Tmp.boneIndex = nullptr;
}

PMX::Morph::Material::Material(size_t MaterialIndexSize)
{
	materialIndex = new unsigned char[MaterialIndexSize];
}

PMX::Morph::Material::~Material()
{
	delete[] materialIndex;
	materialIndex = nullptr;
}

PMX::Morph::Material::Material(Material&& Tmp)
	:materialIndex(Tmp.materialIndex), offsetFormat(Tmp.offsetFormat),
	duffse(Tmp.duffse), specular(Tmp.specular), specular_c(Tmp.specular_c),
	ambient(Tmp.ambient), edgeColor(Tmp.edgeColor), edgeSize(Tmp.edgeSize),
	tex_c(Tmp.tex_c), sphere_c(Tmp.sphere_c), toonTex_c(Tmp.toonTex_c)
{
	Tmp.materialIndex = nullptr;
}

PMX::Morph::Group::Group(size_t MorphIndexSize)
{
	morphIndex = new unsigned char[MorphIndexSize];
}

PMX::Morph::Group::~Group()
{
	delete[] morphIndex;
	morphIndex = nullptr;
}

PMX::Morph::Group::Group(Group&& Tmp)
	:morphIndex(Tmp.morphIndex), rate(Tmp.rate)
{
	Tmp.morphIndex = nullptr;
}

PMX::PMXRigidBody::PMXRigidBody(size_t BoneIndexSize)
{
	boneIndex = new unsigned char[BoneIndexSize];
}

PMX::PMXRigidBody::~PMXRigidBody()
{
	delete[] boneIndex;
	boneIndex = nullptr;
}

PMX::PMXRigidBody::PMXRigidBody(PMXRigidBody&& Tmp)
	:name_JP(Tmp.name_JP), name_EN(Tmp.name_EN),
	boneIndex(Tmp.boneIndex), group(Tmp.group), notCollisionGroupFlag(Tmp.notCollisionGroupFlag),
	shape(Tmp.shape), size(Tmp.size), pos(Tmp.pos), rotate(Tmp.rotate),
	mass(Tmp.mass), moveAttenuation(Tmp.moveAttenuation), rotateAttenuation(Tmp.rotateAttenuation),
	rebound(Tmp.rebound), friction(Tmp.friction), physicalOperation(Tmp.physicalOperation)
{
	Tmp.boneIndex = nullptr;
}

PMX::PMXJoint::JointInfo::JointInfo(size_t RigidBodyIndexSize)
{
	rigidBodyIndexA = new unsigned char[RigidBodyIndexSize];
	rigidBodyIndexB = new unsigned char[RigidBodyIndexSize];
}

PMX::PMXJoint::JointInfo::~JointInfo()
{
	delete[] rigidBodyIndexA;
	rigidBodyIndexA = nullptr;
	delete[] rigidBodyIndexB;
	rigidBodyIndexB = nullptr;
}

PMX::PMXJoint::JointInfo::JointInfo(JointInfo&& Tmp)
	:rigidBodyIndexA(Tmp.rigidBodyIndexA), rigidBodyIndexB(Tmp.rigidBodyIndexB),
	pos(Tmp.pos),spin(Tmp.spin), 
	moveLimit_Lower(Tmp.moveLimit_Lower), moveLimit_Upper(Tmp.moveLimit_Upper),
	spinLimit_Lower(Tmp.spinLimit_Lower), spinLimit_Upper(Tmp.spinLimit_Upper),
	springConstant_Move(Tmp.springConstant_Move), springConstant_Spin(Tmp.springConstant_Spin)
{
	Tmp.rigidBodyIndexA = nullptr;
	Tmp.rigidBodyIndexB = nullptr;
}

PMX::PMXJoint::~PMXJoint()
{
	delete info;
	info = nullptr;
}