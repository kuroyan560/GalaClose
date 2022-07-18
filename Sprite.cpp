#include "Sprite.h"
#include "d3dx12.h"

#include"PipelineMgr.h"
#include"D3D12App.h"
#include"WinApp.h"

#include"SRVDescMgr.h"

#include"MyFunc.h"

int Sprite::MOUSE_IMG_ID = -1;
std::vector<Sprite*>Sprite::STATIC_SPRITES;

#pragma region パイプライン情報（静的メンバ）

const std::string Sprite_Normal::PIPELINE_KEY = "SPRITE";

void Sprite::CreatePipeline()
{
	const wchar_t* VSname = L"SpriteVertexShader.hlsl";
	const wchar_t* PSname = L"SpritePixelShader.hlsl";
	D3D12_INPUT_ELEMENT_DESC input[] =
	{
		//xyz座標
		{
			"POSITION",		//セマンティック名
			0,				//同じセマンティック名が複数あるときに使うインデックス（０でよい）
			DXGI_FORMAT_R32G32B32_FLOAT,	//要素数とビット数を表す（XYZの３つでfloat型なので R32G32B32_FLOAT)
			0,	//入力スロットインデックス（０でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,	//データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0		//一度に描画するインスタンス数（０でよい）
		},
		//uv座標
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0　レジスタ

	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//定数バッファビューとして初期化(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);	//テクスチャ用

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY, VSname, PSname,
		input, _countof(input), rootparams, _countof(rootparams), D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, false, false);
}
#pragma endregion

#pragma region スプライト基底クラス
Sprite* Sprite::HitCheckAll(XMFLOAT2 ScreenPos, int StartID)
{
	int num = -1;	//探索した数
	int nowID;	//当たり判定を取るID
	if (StartID < 0 || STATIC_SPRITES.size() <= StartID)
	{
		nowID = -1;	//IDの指定が不当だったら先頭から線形探索
	}
	else
	{
		nowID = StartID;	//指定されたIDの次から探索を始める
	}

	while (num != STATIC_SPRITES.size() - 1)	//全部探索終わるまで
	{
		num++;
		nowID++;	//次のIDのスプライトをセット
		if (nowID == STATIC_SPRITES.size())nowID = 0;	//末端まできたら先頭に戻る

		if (nowID == MOUSE_IMG_ID)continue;		//マウスカーソル用のスプライトなら無視
		if (!STATIC_SPRITES[nowID]->draw)continue;	//描画されていないものなら無視

		if (STATIC_SPRITES[nowID]->HitCheck(ScreenPos))
		{
			return STATIC_SPRITES[nowID];	//当たり判定がとれたらそれを返す
		}
	}

	return nullptr;	//何もヒットしなければnullptrを返す
}

void Sprite::RegisterSprite(Sprite* Tmp, bool ForCorsor)
{
	STATIC_SPRITES.push_back(Tmp);
	Tmp->id = STATIC_SPRITES.size() - 1;
	if (ForCorsor)MOUSE_IMG_ID = Tmp->id;
}

void Sprite::VertMapping()
{
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (flipX)	//左右反転
	{
		left *= -1;
		right *= -1;
	}
	if (flipY)	//上下反転
	{
		top *= -1;
		bottom *= -1;
	}

	vertices[LB].pos = { left,bottom,0.0f };		//左下
	vertices[LT].pos = { left,top,0.0f };			//左上
	vertices[RB].pos = { right,bottom,0.0f };		//右下
	vertices[RT].pos = { right,top,0.0f };			//右上

	//頂点バッファへのデータ転送
	SPRITE::Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
}

Sprite::Sprite(std::string Name, int Handle, XMFLOAT2 Size, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint)
{
	name = Name;
	texHandle = Handle;
	size = Size;
	flipX = FlipX;
	flipY = FlipY;
	anchorPoint = AnchorPoint;
	vertices[0] = { {},{0.0f,1.0f} };	//左下
	vertices[1] = { {},{0.0f,0.0f} };	//左上
	vertices[2] = { {},{1.0f,1.0f} };	//右下
	vertices[3] = { {},{1.0f,0.0f} };	//右上

	HRESULT result;

	//頂点バッファ生成
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC(CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices))),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	VertMapping();

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

}

Sprite::Sprite(std::string Name, int Handle, MyDef::int2 Size, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint)
{
	name = Name;
	texHandle = Handle;
	size.x = (float)Size.x * WinApp::Instance()->GetWinDifferRate().x;
	size.y = (float)Size.y * WinApp::Instance()->GetWinDifferRate().y;
	flipX = FlipX;
	flipY = FlipY;
	anchorPoint = AnchorPoint;

	vertices[0] = { {},{0.0f,1.0f} };	//左下
	vertices[1] = { {},{0.0f,0.0f} };	//左上
	vertices[2] = { {},{1.0f,1.0f} };	//右下
	vertices[3] = { {},{1.0f,0.0f} };	//右上

	HRESULT result;

	//頂点バッファ生成
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC(CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices))),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	VertMapping();

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}

bool Sprite::HitCheck(XMFLOAT2 ScreenPos)
{
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (flipX)	//左右反転
	{
		left *= -1;
		right *= -1;
	}
	if (flipY)	//上下反転
	{
		top *= -1;
		bottom *= -1;
	}

	if (ScreenPos.x < left)return false;
	if (right < ScreenPos.x)return false;
	if (ScreenPos.y < bottom)return false;
	if (top < ScreenPos.y)return false;
	return true;
}

void Sprite::ChangeTexExtent(XMFLOAT2 TexPos, XMFLOAT2 TexSize)
{
	D3D12_RESOURCE_DESC texDesc = SRVDescMgr::Instance()->GetTexDesc(this->texHandle);
	float texL = TexPos.x / texDesc.Width;
	float texR = (TexPos.x + TexSize.x) / texDesc.Width;
	float texT = TexPos.y / texDesc.Height;
	float texB = (TexPos.y + TexSize.y) / texDesc.Height;

	vertices[LB].uv = { texL,texB };	//左下
	vertices[LT].uv = { texL,texT };	//左上
	vertices[RB].uv = { texR,texB };	//右下
	vertices[RT].uv = { texR,texT };	//右上

	VertMapping();
}

void Sprite::ChangeSize(XMFLOAT2 Size)
{
	size = Size;

	VertMapping();
}
void Sprite::PreDraw()
{
	PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY);
	SRVDescMgr::Instance()->SetSRV(texHandle, SPRITE::ROOT_PARAM::TEXTURE);
	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);
}

void Sprite::Draw(Sprite_Component* Component)
{
	if (draw && Component->color.color.w != 0.0f)
	{
		Component->SetConstBuff();

		D3D12App::Instance()->GetCmdList()->DrawInstanced(INDEX_NUM, 1, 0, 0);
	}
}
#pragma endregion

#pragma region スプライトのコンポーネント
Sprite_Component::Sprite_Component()
{
	HRESULT result;
	//定義バッファの生成
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC(CD3DX12_RESOURCE_DESC::Buffer((sizeof(constBuff) + 0xff) & ~0xff)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	//定数バッファにデータ転送
	ColorMapping();
	MatMapping();

	transformDirty.AddInspector(&transform.pos.x);
	transformDirty.AddInspector(&transform.pos.y);
	transformDirty.AddInspector(&transform.rotate.x);
	transformDirty.AddInspector(&transform.rotate.y);
	transformDirty.AddInspector(&transform.scale.x);
	transformDirty.AddInspector(&transform.scale.y);
	colorDirty.AddInspector(&color.color.x);
	colorDirty.AddInspector(&color.color.y);
	colorDirty.AddInspector(&color.color.z);
	colorDirty.AddInspector(&color.color.w);
}
void Sprite_Component::DirtyRegister()
{
	transformDirty.Register();
	colorDirty.Register();
}
void Sprite_Component::ColorMapping()
{
	//定数バッファにデータ転送
	SPRITE::Const* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;	//色指定（RGBA）
	constBuff->Unmap(0, nullptr);
}

void Sprite_Component::MatMapping()
{
	//ワールド行列の更新
	transform.mat = XMMatrixIdentity();
	transform.mat = XMMatrixScaling(transform.scale.x, transform.scale.y, 1.0f);
	transform.mat *= XMMatrixRotationZ(XMConvertToRadians(transform.rotate.x));
	transform.mat *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	transform.mat *= XMMatrixRotationY(XMConvertToRadians(transform.rotate.y));
	transform.mat *= XMMatrixTranslation(transform.pos.x, transform.pos.y, 0);

	//定数バッファにデータ転送
	SPRITE::Const* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = transform.mat * WinApp::PARALLEL_MAT_PROJECTION;	//行列の合成
	constBuff->Unmap(0, nullptr);
}

void Sprite_Component::SetConstBuff()
{
	if (colorDirty.Dirty())
	{
		ColorMapping();
	}
	if (transformDirty.Dirty())
	{
		MatMapping();
	}
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(
		SPRITE::ROOT_PARAM::CONST_BUFF, constBuff->GetGPUVirtualAddress());
}
#pragma endregion

#pragma region 通常のスプライト
Sprite_Normal::Sprite_Normal(std::string Name, int Handle, XMFLOAT2 Size,
	XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint)
	:Sprite(Name, Handle, Size, FlipX, FlipY, AnchorPoint)
{
	component.transform.pos = Pos;
	ConstMapping();
}

Sprite_Normal::Sprite_Normal(std::string Name, int Handle, MyDef::int2 Size,
	XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint)
	: Sprite(Name, Handle, Size, FlipX, FlipY, AnchorPoint)
{
	component.transform.pos = Pos;
	ConstMapping();
}

void Sprite_Normal::Draw()
{
	PreDraw();
	Sprite::Draw(&component);
}

void Sprite_Normal::ChangeColor(int R, int G, int B)
{
	component.color.color.x = (float)R / 255.0f;
	component.color.color.y = (float)G / 255.0f;
	component.color.color.z = (float)B / 255.0f;
}

bool Sprite_Normal::HitCheck(XMFLOAT2 ScreenPos)
{
	float left = (0.0f - anchorPoint.x) * size.x + component.transform.pos.x;
	if (ScreenPos.x < left)return false;

	float right = (1.0f - anchorPoint.x) * size.x + component.transform.pos.x;
	if (right < ScreenPos.x)return false;

	float bottom = (1.0f - anchorPoint.y) * size.y + component.transform.pos.y;
	if (bottom < ScreenPos.y)return false;

	float top = (0.0f - anchorPoint.y) * size.y + component.transform.pos.y;
	if (ScreenPos.y < top)return false;

	return true;
}
void Sprite_Normal::ConstMapping()
{
	component.ColorMapping();
	component.MatMapping();
}
#pragma endregion

#pragma region 画像分割スプライト
Sprite_Divide::Sprite_Divide(std::string Name, MyDef::int2 SplitNum, MyDef::int2 SplitSize, int Handle, XMFLOAT2 Size,
	XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint)
	:Sprite_Normal(Name, Handle, Size, Pos, FlipX, FlipY, AnchorPoint)
{
	totalSplit = { SplitNum.x * SplitSize.x ,SplitNum.y * SplitSize.y };
	splitSize.x = SplitSize.x;
	splitSize.y = SplitSize.y;
}

void Sprite_Divide::SelectImgNum(int X, int Y)
{
	split.x = X;
	split.y = Y;
	float x = splitSize.x * X;
	float y = splitSize.y * Y;

	this->ChangeTexExtent({ x,y }, { splitSize.x,splitSize.y });
}
#pragma endregion