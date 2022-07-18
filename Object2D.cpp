#include "Object2D.h"
#include"D3D12App.h"
#include"PipelineMgr.h"
#include"SRVDescMgr.h"
#include"CameraMgr.h"

const string Object2D::PIPELINE_KEY = "OBJECT_2D";

void Object2D::CreatePipeline()
{
	const wchar_t* VSname = L"2DVertexShader.hlsl";
	const wchar_t* PSname = L"2DPixelShader.hlsl";
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
		//法線ベクトル
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
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

void Object2D::VertMapping()
{
	const float radius_x = size.x / 2;
	const float radius_y = size.y / 2;
	float left = (0.0f - anchorPoint.x) * radius_x;
	float right = (1.0f - anchorPoint.x) * radius_x;
	float bottom = (0.0f - anchorPoint.y) * radius_y;
	float top = (1.0f - anchorPoint.y) * radius_y;
	
	vertices[LB].pos = { left,bottom,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RT].pos = { right,top,0.0f };

	//頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
}

Object2D::Object2D(string Name, XMFLOAT2 Size, XMFLOAT3 Pos, int TexHandle, XMFLOAT2 AnchorPoint, bool BillBoard, bool BillBoardY)
	:texHandle(TexHandle),
	Object3D(Name + "_Obj2D", Pos, XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0))
{
	pos = { Pos.x,Pos.y,Pos.z };
	scale = { 1.0f,1.0f,1.0f };
	rotation = { 0.0f,0.0f,0.0f };
	anchorPoint = { AnchorPoint.x,AnchorPoint.y };

	size = Size;
	sizeDirty.AddInspector(&size.x);
	sizeDirty.AddInspector(&size.y);

	vertices[0] = { {},{0,0,1},{0,1} };
	vertices[1] = { {},{0,0,1},{1,1} };
	vertices[2] = { {},{0,0,1},{0,0} };
	vertices[3] = { {},{0,0,1},{1,0} };

	//頂点バッファ生成
	HRESULT result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
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

	isBillboard = BillBoard;
	isBillboardY = BillBoardY;
}

void Object2D::Draw()
{
	if (isAlive)
	{
		if (matDirty.Dirty() || parentMat != nullptr || CameraMgr::Instance()->Dirty())
		{
			MatConstMapping();
		}
		if (sizeDirty.Dirty())
		{
			VertMapping();
		}

		PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY);

		MatConstSet(0);

		// シェーダリソースビューをセット
		SRVDescMgr::Instance()->SetSRV(texHandle, 1);

		D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

		D3D12App::Instance()->GetCmdList()->DrawInstanced(VERTEX_COUNT, 1, 0, 0);
	}
}

void Object2D::ChangeTexExtent(XMFLOAT2 TexPos, XMFLOAT2 TexSize)
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

Object2D_Divide::Object2D_Divide(std::string Name, MyDef::int2 SplitNum, MyDef::int2 SplitSize, int TexHandle, XMFLOAT2 Size, XMFLOAT3 Pos, XMFLOAT2 AnchorPoint, bool BillBoard, bool BillBoardY)
	:Object2D(Name, Size, Pos, TexHandle, AnchorPoint, BillBoard, BillBoardY)
{
	totalSplit = { SplitNum.x * SplitSize.x ,SplitNum.y * SplitSize.y };
	splitSize.x = SplitSize.x;
	splitSize.y = SplitSize.y;
}

void Object2D_Divide::SelectImgNum(int X, int Y)
{
	split.x = X;
	split.y = Y;
	float x = splitSize.x * X;
	float y = splitSize.y * Y;

	this->ChangeTexExtent({ x,y }, { splitSize.x,splitSize.y });
}
