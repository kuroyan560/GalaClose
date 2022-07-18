#include "PrimitiveParticles.h"
#include"D3D12App.h"
#include"PipelineMgr.h"
#include"CameraMgr.h"

using namespace PRIMITIVE_PARTICLE;
#pragma region �p�C�v���C�����
const std::string PrimitivePaticles::PIPELINE_KEY = "PRIMITIVE_PARTICLE";
void PrimitivePaticles::CreatePipeline()
{
	const wchar_t* VSname = L"PrimitiveParticleVertexShader.hlsl";
	const wchar_t* PSname = L"PrimitiveParticlePixelShader.hlsl";
	const wchar_t* GSname = L"PrimitiveParticleGeometryShader.hlsl";

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
		//��]
		{
			"ROTATE",		//�Z�}���e�B�b�N��
			0,				//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i�O�ł悢�j
			DXGI_FORMAT_R32G32B32_FLOAT,	//�v�f���ƃr�b�g����\���iXYZ�̂R��float�^�Ȃ̂� R32G32B32_FLOAT)
			0,	//���̓X���b�g�C���f�b�N�X�i�O�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,	//�f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^��ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0		//��x�ɕ`�悷��C���X�^���X���i�O�ł悢�j
		},
		//���a
		{
			"RADIUS",		//�Z�}���e�B�b�N��
			0,				//�����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i�O�ł悢�j
			DXGI_FORMAT_R32_FLOAT,	//�v�f���ƃr�b�g����\���iXYZ�̂R��float�^�Ȃ̂� R32G32B32_FLOAT)
			0,	//���̓X���b�g�C���f�b�N�X�i�O�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,	//�f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^��ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0		//��x�ɕ`�悷��C���X�^���X���i�O�ł悢�j

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
		//�v���~�e�B�u�̌`��
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
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//�J����

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY, VSname, PSname, GSname,
		input, _countof(input), rootparams, _countof(rootparams), D3D_PRIMITIVE_TOPOLOGY_POINTLIST, false);
}
#pragma endregion

PrimitivePaticles::PrimitivePaticles()
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(PRIMITIVE_PARTICLE::Vertex) * MAX_PARTICLE_NUM);
	// ���_�o�b�t�@����
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(PRIMITIVE_PARTICLE::Vertex);
}

void PrimitivePaticles::EmitParticle(int EmitNum, float3& Pos)
{
	for (int i = 0; i < EmitNum; i++)
	{
		if (deadParticles.empty())break;
		//�p�[�e�B�N���𔭐�������
		deadParticles.front().lock()->Emit(Pos);
		//�X�|�[�������p�[�e�B�N���𐶑��p�[�e�B�N�����X�g�Ɉړ�
		aliveParticles.splice_after(aliveParticles.before_begin(), std::move(deadParticles), deadParticles.before_begin());
	}
}

void PrimitivePaticles::Init()
{
	UniqueInit();
	while (!aliveParticles.empty())
	{
		//�����p�[�e�B�N�������S�p�[�e�B�N�����X�g�Ɉړ�
		deadParticles.splice_after(deadParticles.before_begin(), std::move(aliveParticles), aliveParticles.before_begin());
	}
}

void PrimitivePaticles::Update()
{
	UniqueUpdate();

	//�S�p�[�e�B�N���X�V
	for (auto itr = aliveParticles.begin(); itr != aliveParticles.end(); itr++)
	{
		(*itr).lock()->Update();
	}

	//�������Ɏ���ł�����R���e�i�������
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
		//���_���X�g���̒��_�o�b�t�@�}�b�s���O
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
				vertMap++;	//���̒��_��
			}
			vertBuff->Unmap(0, nullptr);
		}
	}

	//�p�C�v���C��
	PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY);

	//�J����
	CameraMgr::Instance()->SetCamConstBuff(PRIMITIVE_PARTICLE::ROOT_PARAM::CAMERA);

	//���_�o�b�t�@�r���[
	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	//�`��
	UINT num = (UINT)std::distance(aliveParticles.begin(), aliveParticles.end());
	D3D12App::Instance()->GetCmdList()->DrawInstanced(num, 1, 0, 0);
}
