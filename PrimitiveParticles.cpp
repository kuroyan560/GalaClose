#include "PrimitiveParticles.h"
#include"D3D12App.h"
#include"PipelineMgr.h"
#include"CameraMgr.h"

using namespace PRIMITIVE_PARTICLE;
#pragma region パイプライン情報
const std::string PrimitivePaticles::PIPELINE_KEY = "PRIMITIVE_PARTICLE";
void PrimitivePaticles::CreatePipeline()
{
	const wchar_t* VSname = L"PrimitiveParticleVertexShader.hlsl";
	const wchar_t* PSname = L"PrimitiveParticlePixelShader.hlsl";
	const wchar_t* GSname = L"PrimitiveParticleGeometryShader.hlsl";

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
		//回転
		{
			"ROTATE",		//セマンティック名
			0,				//同じセマンティック名が複数あるときに使うインデックス（０でよい）
			DXGI_FORMAT_R32G32B32_FLOAT,	//要素数とビット数を表す（XYZの３つでfloat型なので R32G32B32_FLOAT)
			0,	//入力スロットインデックス（０でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,	//データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0		//一度に描画するインスタンス数（０でよい）
		},
		//半径
		{
			"RADIUS",		//セマンティック名
			0,				//同じセマンティック名が複数あるときに使うインデックス（０でよい）
			DXGI_FORMAT_R32_FLOAT,	//要素数とビット数を表す（XYZの３つでfloat型なので R32G32B32_FLOAT)
			0,	//入力スロットインデックス（０でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,	//データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0		//一度に描画するインスタンス数（０でよい）

		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//プリミティブの形状
		{
			"PRIMITIVE",
			0,
			DXGI_FORMAT_R16_UINT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	CD3DX12_ROOT_PARAMETER rootparams[1];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//カメラ

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY, VSname, PSname, GSname,
		input, _countof(input), rootparams, _countof(rootparams), D3D_PRIMITIVE_TOPOLOGY_POINTLIST, false);
}
#pragma endregion

PrimitivePaticles::PrimitivePaticles()
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(PRIMITIVE_PARTICLE::Vertex) * MAX_PARTICLE_NUM);
	// 頂点バッファ生成
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(PRIMITIVE_PARTICLE::Vertex);
}

void PrimitivePaticles::EmitParticle(int EmitNum, float3& Pos)
{
	for (int i = 0; i < EmitNum; i++)
	{
		if (deadParticles.empty())break;
		//パーティクルを発生させる
		deadParticles.front().lock()->Emit(Pos);
		//スポーンしたパーティクルを生存パーティクルリストに移動
		aliveParticles.splice_after(aliveParticles.before_begin(), std::move(deadParticles), deadParticles.before_begin());
	}
}

void PrimitivePaticles::Init()
{
	UniqueInit();
	while (!aliveParticles.empty())
	{
		//生存パーティクルを死亡パーティクルリストに移動
		deadParticles.splice_after(deadParticles.before_begin(), std::move(aliveParticles), aliveParticles.before_begin());
	}
}

void PrimitivePaticles::Update()
{
	UniqueUpdate();

	//全パーティクル更新
	for (auto itr = aliveParticles.begin(); itr != aliveParticles.end(); itr++)
	{
		(*itr).lock()->Update();
	}

	//もう既に死んでいたらコンテナから消す
	aliveParticles.remove_if(
		[&](std::weak_ptr<Particle> x)
		{
			if (!x.lock()->isAlive())
			{
				deadParticles.push_front(x);
			}
			return !x.lock()->isAlive();
		});
}

void PrimitivePaticles::Draw()
{
	if (!aliveParticles.empty())
	{
		HRESULT result;
		//頂点リスト内の頂点バッファマッピング
		PRIMITIVE_PARTICLE::Vertex* vertMap = nullptr;
		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result))
		{
			for (auto itr = aliveParticles.begin(); itr != aliveParticles.end(); itr++)
			{
				vertMap->primitiveType = (*itr).lock()->vertex.primitiveType;
				vertMap->pos = (*itr).lock()->vertex.pos;
				vertMap->rotate = (*itr).lock()->vertex.rotate;
				vertMap->radius = (*itr).lock()->vertex.radius;
				vertMap->color = (*itr).lock()->vertex.color;
				vertMap++;	//次の頂点へ
			}
			vertBuff->Unmap(0, nullptr);
		}
	}

	//パイプライン
	PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY);

	//カメラ
	CameraMgr::Instance()->SetCamConstBuff(PRIMITIVE_PARTICLE::ROOT_PARAM::CAMERA);

	//頂点バッファビュー
	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	//描画
	UINT num = (UINT)std::distance(aliveParticles.begin(), aliveParticles.end());
	D3D12App::Instance()->GetCmdList()->DrawInstanced(num, 1, 0, 0);
}
