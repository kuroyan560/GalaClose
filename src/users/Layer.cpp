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
		//xyz���W
		{
			"POSITION",		//�Z�}���e�B�b�N��
			0,				//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i�O�ł悢�j
			DXGI_FORMAT_R32G32B32_FLOAT,	//�v�f���ƃr�b�g����\���iXYZ�̂R��float�^�Ȃ̂� R32G32B32_FLOAT)
			0,	//���̓X���b�g�C���f�b�N�X�i�O�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,	//�f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^��ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0		//��x�ɕ`�悷��C���X�^���X���i�O�ł悢�j
		},
	};

	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�F�Ɖ������ݗ�)
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�J����)

	//�ʓ\��p�p�C�v���C��
	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_MESH,
		VSname, PSname, GSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�A�E�g���C���G�b�W�p�p�C�v���C��
	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_LINE,
		VSname, outlinePSname, outlineGSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, false);
}

Layer::Layer(const float& StartAngle, const float& EndAngle, const int& LayerNum, const unsigned int& Color, const float& PushedRate)
	:innerRadius(LAYER_OFFSET_RADIUS* (LayerNum - 1) + INNER_SPACE_RADIUS), outerRadius(LAYER_OFFSET_RADIUS* LayerNum + INNER_SPACE_RADIUS)
{
	//���g�̃��C���[�ԍ�
	layerNum = LayerNum;

	//���g�̐F
	color = Color;

	//�p�x�����炵�Ă����߂̃I�t�Z�b�g
	const float angleOffset = (EndAngle - StartAngle) / (float)OUTER_CIRCLE_VERTEX_NUM;

	//���_
	for (int i = 0; i < 2; ++i) 	//����or��O
	{
		Vertex vertex;
		vertex.pos.z = PANEL_HALF_HEIGHT * (i == 0 ? 1 : -1);

		//�p�x
		float tmpAngle = StartAngle;
		for (int j = 0; j < OUTER_CIRCLE_VERTEX_NUM * 2 + 2; ++j)
		{
			float tmpRadius = (j % 2 == 0) ? innerRadius : outerRadius; //�����̂Ƃ����~�A��̂Ƃ��O�~

			vertex.pos.x = cos(tmpAngle) * tmpRadius;
			vertex.pos.y = -sin(tmpAngle) * tmpRadius;

			if (j % 2 != 0)	//��̂Ƃ�
			{
				//���v���Ɋp�x�����炵�Ă���
				tmpAngle += angleOffset;
			}

			//���_���v�b�V��
			vertices.emplace_back(vertex);
		}
	}

	//�C���f�b�N�X(�ʓ\��p�j
	//�O�ʂƉ���
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
	//���~�̌ʂƊO�~�̌�
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

	//�C���f�b�N�X(�A�E�g���C���G�b�W�p�j
	//�p�l���������C��
	outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 4 + 2);
	outLineIndices.emplace_back(OUTER_CIRCLE_VERTEX_NUM * 4 + 2 + 1);
	outLineIndexCount[OUTLINE_SPLIT_LINE] += 2;

	//�O�~
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
	// ���_�o�b�t�@����
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	if (FAILED(result))
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "���_�o�b�t�@�̍쐬�Ɏ��s");
		DebugBreak();
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex* vertMap = nullptr;
	size_t offsetSize = sizeof(Vertex);
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);

	// �C���f�b�N�X�o�b�t�@�����i�ʓ\��p�j
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
		WinApp::Instance()->CreateMessageWindow("ERROR", "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s");
		DebugBreak();
	}

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = meshIndexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(meshIndices.begin(), meshIndices.end(), indexMap);
		meshIndexBuff->Unmap(0, nullptr);
	}

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	meshIbView.BufferLocation = meshIndexBuff->GetGPUVirtualAddress();
	meshIbView.Format = DXGI_FORMAT_R16_UINT;
	meshIbView.SizeInBytes = sizeIB;

	// �C���f�b�N�X�o�b�t�@�����i�A�E�g���C���G�b�W�p�j
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
		WinApp::Instance()->CreateMessageWindow("ERROR", "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s");
		DebugBreak();
	}

	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	indexMap = nullptr;
	result = outLineIndexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(outLineIndices.begin(), outLineIndices.end(), indexMap);
		outLineIndexBuff->Unmap(0, nullptr);
	}

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	outLineIbView.BufferLocation = outLineIndexBuff->GetGPUVirtualAddress();
	outLineIbView.Format = DXGI_FORMAT_R16_UINT;
	outLineIbView.SizeInBytes = sizeIB;

	//�萔�o�b�t�@����
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(Const)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	// �萔�o�b�t�@�փf�[�^�]��
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
	// �萔�o�b�t�@�փf�[�^�]��
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

	//�ʓ\��
	int idxOffset = 0;
	for (int i = 0; i < MESH_TYPE_NUM; ++i)
	{
		if (meshIndexCount[i] != 0)
		D3D12App::Instance()->GetCmdList()->DrawIndexedInstanced(meshIndexCount[i], 1, idxOffset, 0, 0);
		idxOffset += meshIndexCount[i];
	}

	//�A�E�g���C���G�b�W
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
