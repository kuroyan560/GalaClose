#include "Particles.h"
#include"D3D12App.h"
#include"PipelineMgr.h"
#include"CameraMgr.h"
#include"SRVDescMgr.h"

using namespace PARTICLE;

#pragma region パイプライン情報
const std::string Particles::PIPELINE_KEY = "PARTICLE";
void Particles::CreatePipeline()
{
	const wchar_t* VSname = L"ParticleVertexShader.hlsl";
	const wchar_t* PSname = L"ParticlePixelShader.hlsl";
	const wchar_t* GSname = L"ParticleGeometryShader.hlsl";

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
		//パーティクルのスケール
		{
			"SCALE",		//セマンティック名
			0,				//同じセマンティック名が複数あるときに使うインデックス（０でよい）
			DXGI_FORMAT_R32G32_FLOAT,	//要素数とビット数を表す（XYZの３つでfloat型なので R32G32B32_FLOAT)
			0,	//入力スロットインデックス（０でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,	//データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0		//一度に描画するインスタンス数（０でよい）

		},
		//パーティクル画像のインデックス番号
		{
			"INDEX",
			0,
			DXGI_FORMAT_R16G16_UINT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//uv座標
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0　レジスタ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);	//テクスチャ用
	rootparams[1].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//定数バッファ

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY, VSname, PSname, GSname,
		input, _countof(input), rootparams, _countof(rootparams), D3D_PRIMITIVE_TOPOLOGY_POINTLIST, true, false);
}

#pragma endregion

Particles::Particles(int ImgHandle, unsigned short X, unsigned short Y)
	:img(ImgHandle), imgIndex{ X,Y }
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(PARTICLE::Vertex) * MAX_PARTICLE_NUM);
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
	vbView.StrideInBytes = sizeof(PARTICLE::Vertex);

	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(PARTICLE::Const) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	// 定数バッファへデータ転送
	PARTICLE::Const* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->viewProjMat = CameraMgr::Instance()->GetView() * CameraMgr::Instance()->GetProjection();
		constMap->billboardMat = CameraMgr::Instance()->GetBillboardMatrix();
		constMap->imgSplitX = X;
		constMap->imgSplitY = Y;
		constBuff->Unmap(0, nullptr);
	}
}

void Particles::EmitParticle(int EmitNum, float3 Pos)
{
	for (int i = 0; i < EmitNum; i++)
	{
		if (particlesList.empty())
		{
			particles[0]->Emit(Pos, imgIndex[0], imgIndex[1]);
			particlesList.emplace_front(particles[0]);
		}

		bool flag;
		int index = 0;
		do
		{
			flag = false;
			for (auto itr : particlesList)
			{
				//既に排出済のものは無視
				if (itr == particles[index])
				{
					flag = true;
					break;
				}
			}
			index++;
		} while (flag);
		index--;
		particles[index]->Emit(Pos, imgIndex[0], imgIndex[1]);
		particlesList.emplace_front(particles[index]);
	}
}

void Particles::Init()
{
	UniqueInit();
	particlesList.clear();
}

void Particles::Update()
{
	UniqueUpdate();

	//全パーティクル更新
	for (auto itr = particlesList.begin(); itr != particlesList.end(); itr++)
	{
		(*itr)->Update();
	}

	//もう既に死んでいたらコンテナ消す
	particlesList.remove_if(
		[](Particle* x)
	{
		return !x->isAlive();
	});
}

void Particles::Draw()
{
	if (!particlesList.empty())
	{
		HRESULT result;
		// 定数バッファへデータ転送
		PARTICLE::Const* constMap = nullptr;
		result = constBuff->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result)) {
			constMap->viewProjMat = CameraMgr::Instance()->GetView() * CameraMgr::Instance()->GetProjection();
			constMap->billboardMat = CameraMgr::Instance()->GetBillboardMatrix();
			constBuff->Unmap(0, nullptr);
		}

		//頂点リスト内の頂点バッファマッピング
		PARTICLE::Vertex* vertMap = nullptr;
		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result))
		{
			for (auto itr = particlesList.begin(); itr != particlesList.end(); itr++)
			{
				vertMap->pos = (*itr)->vertex.pos;
				vertMap->color = (*itr)->vertex.color;
				vertMap->scale = (*itr)->vertex.scale;
				vertMap->index[0] = (*itr)->vertex.index[0];
				vertMap->index[1] = (*itr)->vertex.index[1];
				vertMap++;	//次の頂点へ
			}
			vertBuff->Unmap(0, nullptr);
		}

		//パイプライン
		PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY);

		//テクスチャ（SRV）
		SRVDescMgr::Instance()->SetSRV(img, ROOT_PARAM::TEXTURE);

		//定数バッファ
		D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(ROOT_PARAM::CONST_BUFF, constBuff->GetGPUVirtualAddress());

		//頂点バッファビュー
		D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

		//描画
		D3D12App::Instance()->GetCmdList()->DrawInstanced((UINT)std::distance(particlesList.begin(), particlesList.end()), 1, 0, 0);
	}
}
