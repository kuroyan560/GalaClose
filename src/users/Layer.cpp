#include "Layer.h"
#include"Constants.h"
#include"d3dx12.h"
#include"../../PipelineMgr.h"
#include"../../D3D12App.h"
#include"../../WinApp.h"
#include"../../CameraMgr.h"
#include"GameOver.h"

const std::string Layer::PIPELINE_KEY_MESH = "LAYER_MESH";
const std::string Layer::PIPELINE_KEY_LINE = "LAYER_LINE";
void Layer::CreatePipeline()
{
	const wchar_t* VSname = L"src/users/hlsl/LayerVertexShader.hlsl";
	const wchar_t* PSname = L"src/users/hlsl/LayerPixelShader.hlsl";
	const wchar_t* outlinePSname = L"src/users/hlsl/LayerOutLinePixelShader.hlsl";
	const wchar_t* GSname = L"src/users/hlsl/LayerGeometryShader.hlsl";
	const wchar_t* outlineGSname = L"src/users/hlsl/LayerOutLineGeometryShader.hlsl";

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
	};

	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//定数バッファ(色と押し込み率)
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);	//定数バッファ(カメラ)

	//面貼り用パイプライン
	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_MESH,
		VSname, PSname, GSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//アウトラインエッジ用パイプライン
	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_LINE,
		VSname, outlinePSname, outlineGSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, false);
}

Layer::Layer(const float& StartAngle, const float& EndAngle, const int& LayerNum, const unsigned int& Color, const float& PushedRate)
	:innerRadius(LAYER_OFFSET_RADIUS* (LayerNum - 1) + INNER_SPACE_RADIUS), outerRadius(LAYER_OFFSET_RADIUS* LayerNum + INNER_SPACE_RADIUS)
{
	//自身のレイヤー番号
	layerNum = LayerNum;

	//自身の色
	color = Color;

	//角度をずらしてくためのオフセット
	const float angleOffset = (EndAngle - StartAngle) / (float)OUTER_CIRCLE_VERTEX_NUM;

	//頂点
	for (int i = 0; i < 2; ++i) 	//奥面or手前
	{
		Vertex vertex;
		vertex.pos.z = PANEL_HALF_HEIGHT * (i == 0 ? 1 : -1);

		//角度
		float tmpAngle = StartAngle;
		for (int j = 0; j < OUTER_CIRCLE_VERTEX_NUM * 2 + 2; ++j)
		{
			float tmpRadius = (j % 2 == 0) ? innerRadius : outerRadius; //偶数のとき内円、奇数のとき外円

			vertex.pos.x = cos(tmpAngle) * tmpRadius;
			vertex.pos.y = -sin(tmpAngle) * tmpRadius;

			if (j % 2 != 0)	//奇数のとき
			{
				//時計回りに角度をずらしていく
				tmpAngle += angleOffset;
			}

			//頂点をプッシュ
			vertices.emplace_back(vertex);
		}
	}

	//インデックス(面貼り用）
	//前面と奥面
	for (int i = 0; i <= NEAR_FACE; ++i)
	{
		int offsetIdx = i * (OUTER_CIRCLE_VERTEX_NUM * 2 + 2);
		for (int j = 0; j < OUTER_CIRCLE_VERTEX_NUM * 2; j += 6)
		{
			if (i == 0)
			{
				meshIndices.emplace_back(j + offsetIdx + 1);
				meshIndices.emplace_back(j + offsetIdx);
				meshIndices.emplace_back(j + offsetIdx + 3);

				meshIndices.emplace_back(j + offsetIdx + 2);
				meshIndices.emplace_back(j + offsetIdx + 5);
				meshIndices.emplace_back(j + offsetIdx + 4);
			}
			else
			{
				meshIndices.emplace_back(j + offsetIdx);
				meshIndices.emplace_back(j + offsetIdx + 1);
				meshIndices.emplace_back(j + offsetIdx + 2);

				meshIndices.emplace_back(j + offsetIdx + 3);
				meshIndices.emplace_back(j + offsetIdx + 4);
				meshIndices.emplace_back(j + offsetIdx + 5);
			}
			meshIndexCount[i] += 6;
		}
		meshIndices.pop_back();
		meshIndices.pop_back();
		meshIndexCount[i] -= 2;
	}
	//内円の弧と外円の弧
	for (int i = OUTER_CIRC_FACE; i <= OUTER_CIRC_FACE; ++i)
	{
		int offsetIdx = i - INNER_CIRC_FACE;
		for (int j = 0; j < OUTER_CIRCLE_VERTEX_NUM * 2; j += 6)
		{
			meshIndices.emplace_back(j + offsetIdx + OUTER_CIRCLE_VERTEX_NUM * 2 + 2);
			meshIndices.emplace_back(j + offsetIdx);
			meshIndices.emplace_back(j + offsetIdx + OUTER_CIRCLE_VERTEX_NUM * 2 + 2 + 2);

			meshIndices.emplace_back(j + offsetIdx + 2);
			//meshIndices.emplace_back(j + offsetIdx + OUTER_CIRCLE_VERTEX_NUM * 2 + 2 + 4);
			//meshIndices.emplace_back(j + offsetIdx + 4);

			meshIndexCount[i] += 4;
		}
	}

	//インデックス(アウトラインエッジ用）
	//パネル分割ライン
	outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 4 + 2);
	outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 4 + 2 + 1);
	outLineIndexCount[OUTLINE_SPLIT_LINE] += 2;

	//外円
	for (int i = 0; i < OUTER_CIRCLE_VERTEX_NUM + 1; ++i)
	{
		outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 2 + 2 + i * 2);
		outLineIndexCount[OUTLINE_OUTER_CIRC]++;
	}

	//outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 2);
	//outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 2+ 1);
	//outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 4 + 3);
	//outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 4 + 2);
	//outLineIndexCount[OUTLINE_END_SIDE] += 4;

	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	// 頂点バッファ生成
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	if (FAILED(result))
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "頂点バッファの作成に失敗");
		DebugBreak();
	}

	// 頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	size_t offsetSize = sizeof(Vertex);
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	// インデックスバッファ生成（面貼り用）
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * meshIndices.size());
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&meshIndexBuff));

	if (FAILED(result))
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "インデックスバッファの作成に失敗");
		DebugBreak();
	}

	//インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = meshIndexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(meshIndices.begin(), meshIndices.end(), indexMap);
		meshIndexBuff->Unmap(0, nullptr);
	}

	//インデックスバッファビューの作成
	meshIbView.BufferLocation = meshIndexBuff->GetGPUVirtualAddress();
	meshIbView.Format = DXGI_FORMAT_R16_UINT;
	meshIbView.SizeInBytes = sizeIB;

	// インデックスバッファ生成（アウトラインエッジ用）
	sizeIB = static_cast<UINT>(sizeof(unsigned short) * outLineIndices.size());
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&outLineIndexBuff));

	if (FAILED(result))
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "インデックスバッファの作成に失敗");
		DebugBreak();
	}

	//インデックスバッファへのデータ転送
	indexMap = nullptr;
	result = outLineIndexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(outLineIndices.begin(), outLineIndices.end(), indexMap);
		outLineIndexBuff->Unmap(0, nullptr);
	}

	//インデックスバッファビューの作成
	outLineIbView.BufferLocation = outLineIndexBuff->GetGPUVirtualAddress();
	outLineIbView.Format = DXGI_FORMAT_R16_UINT;
	outLineIbView.SizeInBytes = sizeIB;

	//定数バッファ生成
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(Const)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	// 定数バッファへデータ転送
	Const* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->pushedRate = PushedRate;
		constBuff->Unmap(0, nullptr);
	}
}

void Layer::Update(const float& PushedRate)
{
	float offset = 0.0f;
	if (GameOver::Instance()->Flag())
	{
		GameOver::Instance()->AffectPushedRate(0.0f, explosionLayerRateOffset, &offset);
	}
	// 定数バッファへデータ転送
	Const* constMap = nullptr;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->pushedRate = PushedRate + offset;
		constBuff->Unmap(0, nullptr);
	}
}

void Layer::Draw()
{
	PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY_MESH);
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	CameraMgr::Instance()->SetCamConstBuff(1);

	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);
	D3D12App::Instance()->GetCmdList()->IASetIndexBuffer(&meshIbView);

	//面貼り
	int idxOffset = 0;
	for (int i = 0; i < MESH_TYPE_NUM; ++i)
	{
		if (meshIndexCount[i] != 0)
		D3D12App::Instance()->GetCmdList()->DrawIndexedInstanced(meshIndexCount[i], 1, idxOffset, 0, 0);
		idxOffset += meshIndexCount[i];
	}

	//アウトラインエッジ
	PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY_LINE);
	D3D12App::Instance()->GetCmdList()->IASetIndexBuffer(&outLineIbView);

	idxOffset = 0;
	for (int i = 0; i < OUTLINE_TYPE_NUM; ++i)
	{
		if (outLineIndexCount[i] != 0)
			D3D12App::Instance()->GetCmdList()->DrawIndexedInstanced(outLineIndexCount[i], 1, idxOffset, 0, 0);
		idxOffset += outLineIndexCount[i];
	}
}

void Layer::SetExplosionOffset()
{
	explosionLayerRateOffset = MyFunc::GetRand(-6.0f, -3.0f);
}
