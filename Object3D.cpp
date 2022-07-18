#include "Object3D.h"
#include"d3dx12.h"
#include"D3D12App.h"
#include"CameraMgr.h"
#include"MyFunc.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include<stdlib.h>
#include<algorithm>
#include"BaseCollider.h"
#include"CollisionMgr.h"

std::map<std::string, int>Object3D::registeredObjInfo;
std::vector<Object3D*> Object3D::objs;

void Object3D::MatConstMappingAll()
{
 	for (auto obj : objs)
	{
		obj->MatConstMapping();
	}
}

void Object3D::InitAll()
{
	for (auto obj : objs)
	{
		obj->Init();
	}
}

void Object3D::FinalizeAll()
{
	for (auto obj : objs)
	{
		obj->Finalize();
	}
}

Object3D::Object3D(string Name, XMFLOAT3& Pos, XMFLOAT3& Scale, XMFLOAT3& Rotation)
{
	pos = Pos;
	scale = Scale;
	rotation = Rotation;

	if (!objs.empty())
	{
		auto itr = registeredObjInfo.begin();
		for (; itr != registeredObjInfo.end(); itr++)
		{
			if (itr->first == Name)break;
		}
		if (itr == registeredObjInfo.end())
		{
			//まだ登録していないならIDを０で初期化して登録
			registeredObjInfo[Name] = 0;
		}
	}

	char num[256];
	sprintf_s(num, "%d", registeredObjInfo[Name]);
	string str(num);
	name = Name + "(" + str + ")";
	registeredObjInfo[Name]++;

	HRESULT result;
	// 定数バッファの生成
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Object3D::Const) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	MatConstMapping();

	matDirty.AddInspector(&pos.x);
	matDirty.AddInspector(&pos.y);
	matDirty.AddInspector(&pos.z);
	matDirty.AddInspector(&rotation.x);
	matDirty.AddInspector(&rotation.y);
	matDirty.AddInspector(&rotation.z);
	matDirty.AddInspector(&scale.x);
	matDirty.AddInspector(&scale.y);
	matDirty.AddInspector(&scale.z);
	matDirty.AddInspector(&up.x);
	matDirty.AddInspector(&up.y);
	matDirty.AddInspector(&up.z);
}
Object3D::~Object3D()
{
	MyFunc::SafetyDelete(actEditors);
}

void Object3D::MatConstSet(int RootParamIndex)
{
	// 行列の定数バッファをセット
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(RootParamIndex, constBuff->GetGPUVirtualAddress());
}

void Object3D::Init()
{
	initPos = pos;
	initScale = scale;
	initRotation = rotation;
	UniqueInit();
}

void Object3D::Update()
{
	if (isAlive)
	{
		UniqueUpdate();
		if (matDirty.Dirty())
		{
			MatConstMapping();
		}
	}
}

void Object3D::Finalize()
{
	pos = initPos;
	scale = initScale;
	rotation = initRotation;
	isAlive = true;
	UniqueFinalize();
}

void Object3D::MatConstMapping()
{
	/*
	* YとZの情報を残したい　→　YとZからXを求める
	*/
	XMVECTOR axisY = XMLoadFloat3(&up);
	assert(!XMVector3Equal(axisY, XMVectorZero()));
	assert(!XMVector3IsInfinite(axisY));
	//Z軸
	XMFLOAT3 z = XMFLOAT3(0, 0, 1);

	if (up.x == z.x && up.y == z.y && up.z == z.z)
	{
		z = XMFLOAT3(0, -1, 0);
	}

	XMVECTOR axisZ = XMLoadFloat3(&z);
	//X軸（右方向）
	XMVECTOR axisX;
	//X軸は上方向→Z軸の外積で求まる
	axisX = XMVector3Cross(axisY, axisZ);
	//ベクトルを正規化
	axisX = XMVector3Normalize(axisX);

	axisZ = XMVector3Cross(axisX, axisY);

	//Y軸はZ軸→X軸の外積で求まる
	//axisY = XMVector3Cross(axisZ, axisX);

	//カメラ回転行列
	XMMATRIX matSpin;
	//座標系→ワールド座標系の変換行列
	matSpin.r[0] = axisX;
	matSpin.r[1] = axisY;
	matSpin.r[2] = axisZ;
	matSpin.r[3] = XMVectorSet(0, 0, 0, 1);

	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);

	// ワールド行列の合成
	matWorld = XMMatrixIdentity(); // 変形をリセット
	matWorld *= matScale * matSpin * matRot * matTrans;

	if (isBillboard || isBillboardY)
	{
		XMMATRIX matBillboard;
		if (isBillboard)
		{
			matBillboard = CameraMgr::Instance()->GetBillboardMatrix();
		}
		else if (isBillboardY)
		{
			matBillboard = CameraMgr::Instance()->GetBillboardMatrixY();
		}
		matWorld = XMMatrixIdentity();
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド行列に回転を反映
		matWorld *= matBillboard;
		matWorld *= matTrans; // ワールド行列に平行移動を反映
	}

	// 親オブジェクトがあれば
	if (parentMat != nullptr) 
	{
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= *parentMat;
	}
	const XMMATRIX& viewMat = CameraMgr::Instance()->GetView();
	const XMMATRIX& projMat = CameraMgr::Instance()->GetProjection();
	const XMFLOAT3& cameraPos = CameraMgr::Instance()->GetPos();

	// 定数バッファへデータ転送
	Object3D::Const* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->viewMat = viewMat;
	constMap->projMat = projMat;
	constMap->worldMat = matWorld;
	constMap->cameraPos = cameraPos;
	constBuff->Unmap(0, nullptr);
}

void Object3D::CreateActiveEditor(std::string EditorName)
{
	actEditors = new ActEditors_Obj3D(EditorName, &this->pos, &this->scale, &this->rotation);
}

void Object3D::DeleteActiveEditor()
{
	//if (Save)actEditors->SaveAE();
	MyFunc::SafetyDelete(actEditors);
}

bool Object3D::LoadActive(std::string ActiveName, bool Save)
{
	//ロードする予定のアクティビティエディタデータがそもそも存在するか
	if (!MyFunc::ExistFile(ActivityEditor::DIRECTORY_PATH + ActiveName + ActEditors_Obj3D::EXTENSION))
	{
		return false;	//存在しなければ何もしない
	}

	//もし生成されていないようなら生成
	if (actEditors == nullptr)CreateActiveEditor(ActiveName);
	//生成されているなら
	else
	{
		//セーブする設定ならaeに出力
		if (Save)
		{
			actEditors->SaveAE();
		}
	}

	//アクティブのロード
	actEditors->LoadAE(ActiveName);

	MatConstMapping();

	return true;
}

void Object3D::SetActive(ActEditors_Obj3D* Ptr)
{
	actEditors = Ptr;

	if (Ptr != nullptr)
	{
		actEditors->Init();

		MatConstMapping();
	}
}