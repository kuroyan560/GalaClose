#include "Model_HSM.h"
using namespace HSM;
#include"D3D12App.h"
#include"WinApp.h"

#include"PipelineMgr.h"

#include"CameraMgr.h"
#include"Material_HSMA.h"

#include"BoneTree.h"

#pragma region �p�C�v���C�����
const string Model_HSM::PIPELINE_KEY = "MODEL_HSM";
const string Model_HSM::PIPELINE_KEY_NON_CULLING = "MODEL_HSM_NONE_CULLING";
//const string Model_HSM::PIPELINE_KEY_WIRE = "MODEL_HSM_WIRE";
void Model_HSM::CreatePipeline()
{
	const wchar_t* VSname = L"HSMVertexShader.hlsl";
	const wchar_t* PSname = L"HSMPixelShader.hlsl";
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
		//�@��
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//uv���W
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//�{�[���ԍ�
		{
			"BONE_NO",
			0,
			DXGI_FORMAT_R16G16B16A16_SINT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//�E�F�C�g
		{
			"WEIGHT",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		////�֊s���t���O
		//{
		//	"EDGE_FLG",
		//	0,
		//	DXGI_FORMAT_R8_UINT,
		//	0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		//	0
		//}
	};

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0�@���W�X�^
	CD3DX12_DESCRIPTOR_RANGE descRangeSRVt1;
	descRangeSRVt1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1�@���W�X�^
	CD3DX12_DESCRIPTOR_RANGE descRangeSRVt2;
	descRangeSRVt2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);	//t2�@���W�X�^
	CD3DX12_DESCRIPTOR_RANGE descRangeSRVt3;
	descRangeSRVt3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);	//t3�@���W�X�^

	CD3DX12_ROOT_PARAMETER rootparams[9] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�}�X�^�[�J���[���[�h)
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�}�e���A��)
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);	//�e�N�X�`���p
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRVt1, D3D12_SHADER_VISIBILITY_ALL);	//sph
	rootparams[4].InitAsDescriptorTable(1, &descRangeSRVt2, D3D12_SHADER_VISIBILITY_ALL);	//spa
	rootparams[5].InitAsDescriptorTable(1, &descRangeSRVt3, D3D12_SHADER_VISIBILITY_ALL);	//�g�D�[���p
	rootparams[6].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�J����)
	rootparams[7].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(���[���h�s��)
	rootparams[8].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�{�[���s��)

	D3D12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���J��Ԃ�
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//�c�J��Ԃ�
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���s���J��Ԃ�
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;	//��Ԃ��Ȃ��i�j�A���X�g�l�C�o�[�j
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;	//�~�b�v�}�b�v�ő�l
	samplerDesc[0].MinLOD = 0.0f;	//�~�b�v�}�b�v�ŏ��l
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplerDesc[0].ShaderRegister = 0;

	samplerDesc[1] = samplerDesc[0];
	samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	//�J��Ԃ��Ȃ�
	samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].ShaderRegister = 1;

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY,
		VSname, PSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		true, true,
		samplerDesc, _countof(samplerDesc));

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_NON_CULLING,
		VSname, PSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		true,true,
		samplerDesc, _countof(samplerDesc));

	//PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_WIRE,
	//	VSname, PSname,
	//	input, _countof(input),
	//	rootparams, _countof(rootparams),
	//	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	//	true, true,
	//	samplerDesc, _countof(samplerDesc));
}
#pragma endregion

#pragma region Model_HSM
Model_HSM::Model_HSM(bool Culling) :culling(Culling)
{
}

void Model_HSM::CreateBuff()
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(HSM::Vertex) * vertices.size());
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
	HSM::Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned int) * indices.size());
	// �C���f�b�N�X�o�b�t�@����
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	if (FAILED(result))
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s");
		DebugBreak();
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	if (head.byteSize[INDEX_BYTE_SIZE::VERTEX_SIZE] == 1)
	{
		unsigned char* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result)) {
			std::copy(indices.begin(), indices.end(), indexMap);
			indexBuff->Unmap(0, nullptr);
		}
	}
	else if (head.byteSize[INDEX_BYTE_SIZE::VERTEX_SIZE] == 2)
	{
		unsigned short* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result)) {
			std::copy(indices.begin(), indices.end(), indexMap);
			indexBuff->Unmap(0, nullptr);
		}
	}
	else if (head.byteSize[INDEX_BYTE_SIZE::VERTEX_SIZE] == 4)
	{
		unsigned int* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result)) {
			std::copy(indices.begin(), indices.end(), indexMap);
			indexBuff->Unmap(0, nullptr);
		}
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//�}�e���A����GPU���\�[�X
	for (int i = 0; i < materials.size(); i++)
	{
		materials[i].CreateBuff();
	}
}

void Model_HSM::CreateBoneTree()
{
	if (skeleton.bones.empty())return;

	//�C���f�b�N�X�Ɩ��O�̑Ή��֌W�\�z�̂��߂Ɍ�Ŏg��
	std::vector<std::string>boneNames(skeleton.bones.size());

	//�{�[���m�[�h�}�b�v�����
	for (int idx = 0; idx < skeleton.bones.size(); ++idx)
	{
		auto& bone = skeleton.bones[idx];
		boneNames[idx] = bone.name;
		auto& node = skeleton.boneNodeTable[bone.name];
		node.boneIdx = idx;
		node.startPos = bone.pos;
	}

	//�e�q�֌W�\�z
	for (int i = 0; i < skeleton.bones.size(); ++i)
	{
		auto& bone = skeleton.bones[i];
		//�e�C���f�b�N�X���`�F�b�N(���肦�Ȃ��ԍ��Ȃ�Ƃ΂�)
		if (bone.parent < 0 || skeleton.bones.size() < bone.parent)
		{
			continue;
		}
		auto parentName = boneNames[bone.parent];
		skeleton.boneNodeTable[parentName].children.emplace_back(&skeleton.boneNodeTable[bone.name]);
	}
}

void Model_HSM::DrawIndexedInstanced(BoneGroup* BoneGroup)
{
	//�{�[���萔�o�b�t�@
	if (BoneGroup != nullptr)
	{
		BoneGroup->SetBoneConstBuff(&skeleton, ROOT_PARAM::BONE_MAT);
	}

	//�}�e���A���萔�o�b�t�@
	unsigned int idxOffset = 0;
	for (int i = 0; i < meshes.size(); i++)
	{
		materials[meshes[i].materialIndex].SetMaterialConstBuff(ROOT_PARAM::MATERIAL,
			ROOT_PARAM::NORMAL_TEX, ROOT_PARAM::SPH_TEX, ROOT_PARAM::SPA_TEX, ROOT_PARAM::TOON_TEX);
		D3D12App::Instance()->GetCmdList()->DrawIndexedInstanced(meshes[i].vertexNum, 1, idxOffset, 0, 0);
		idxOffset += meshes[i].vertexNum;
	}
}
void Model_HSM::DrawIndexedInstancedIndividual(Individualinfo_ForIncrease& Info)
{
	Info.component.SetComponentConstBuff();

	//�{�[���萔�o�b�t�@
	Info.boneGroup.SetBoneConstBuff(&this->skeleton,ROOT_PARAM::BONE_MAT);

	//�}�e���A���萔�o�b�t�@
	unsigned int idxOffset = 0;
	for (int i = 0; i < meshes.size(); i++)
	{
		auto result = find(Info.component.nonDrawMaterialName.begin(), Info.component.nonDrawMaterialName.end(),
			meshes[i].name);
		if (result == Info.component.nonDrawMaterialName.end())
		{
			materials[meshes[i].materialIndex].SetMaterialConstBuff(ROOT_PARAM::MATERIAL,
				ROOT_PARAM::NORMAL_TEX, ROOT_PARAM::SPH_TEX, ROOT_PARAM::SPA_TEX, ROOT_PARAM::TOON_TEX);
			D3D12App::Instance()->GetCmdList()->DrawIndexedInstanced(meshes[i].vertexNum, 1, idxOffset, 0, 0);
		}
		idxOffset += meshes[i].vertexNum;
	}
}

#pragma endregion

#pragma region Model_HSM_Component
Model_HSM_Component::Model_HSM_Component()
{
	HRESULT result;
	UINT sizeCB = static_cast<UINT>(sizeof(transform.mat));
	// �g�����X�t�H�[���o�b�t�@����
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeCB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transformBuff));

	TransformMapping();

	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(masterColor) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&colorBuff));

	MasterColorMapping();

	transformDirty.AddInspector(&transform.pos.x);
	transformDirty.AddInspector(&transform.pos.y);
	transformDirty.AddInspector(&transform.pos.z);
	transformDirty.AddInspector(&transform.scale.x);
	transformDirty.AddInspector(&transform.scale.y);
	transformDirty.AddInspector(&transform.scale.z);
	transformDirty.AddInspector(&transform.rotate.x);
	transformDirty.AddInspector(&transform.rotate.y);
	transformDirty.AddInspector(&transform.rotate.z);

	masterColorDirty.AddInspector(&masterColor.color.x);
	masterColorDirty.AddInspector(&masterColor.color.y);
	masterColorDirty.AddInspector(&masterColor.color.z);
	masterColorDirty.AddInspector(&masterColor.color.w);
	masterColorDirty.AddInspector(&masterColor.shadow);
	masterColorDirty.AddInspector(&masterColor.flash);
}

Model_HSM_Component::~Model_HSM_Component()
{
}

void Model_HSM_Component::DirtyRegister()
{
	masterColorDirty.Register();
	transformDirty.Register();
}

void Model_HSM_Component::DirtyRemove()
{
	masterColorDirty.Remove();
	transformDirty.Remove();
}

void Model_HSM_Component::DirtyRecord()
{
	masterColorDirty.Record();
	transformDirty.Record();
}

void Model_HSM_Component::TransformMatUpdate()
{
#pragma region ��x�N�g���̌X���ɉ����ă��[�J�����W���v�Z
	XMVECTOR axisY = XMLoadFloat3(&transform.up);
	assert(!XMVector3Equal(axisY, XMVectorZero()));
	assert(!XMVector3IsInfinite(axisY));
	//Z��
	XMFLOAT3 z = transform.front;
	if (transform.up.x == 0.0f && transform.up.y == 0.0f)
	{
		if (transform.up.z == 1.0f)
		{
			z = XMFLOAT3(0, -1, 0);
		}
		else if (transform.up.z == -1.0f)
		{
			z = XMFLOAT3(0, 1, 0);
		}
	}

	XMVECTOR axisZ = XMLoadFloat3(&z);
	//X���i�E�����j
	XMVECTOR axisX;
	//X���͏������Z���̊O�ςŋ��܂�
	axisX = XMVector3Cross(axisY, axisZ);
	//�x�N�g���𐳋K��
	axisX = XMVector3Normalize(axisX);

	axisZ = XMVector3Cross(axisX, axisY);
#pragma endregion

	HRESULT result;

	XMMATRIX matScale, matSlope, matRot, matTrans;
	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	matRot = XMMatrixIdentity();

	while (360 <= transform.rotate.z)transform.rotate.z -= 360;
	while (transform.rotate.z < 0)transform.rotate.z += 360;
	matRot *= XMMatrixRotationZ(XMConvertToRadians(transform.rotate.z));

	while (360 <= transform.rotate.x)transform.rotate.x -= 360;
	while (transform.rotate.x < 0)transform.rotate.x += 360;
	matRot *= XMMatrixRotationX(XMConvertToRadians(transform.rotate.x));

	while (360 <= transform.rotate.y)transform.rotate.y -= 360;
	while (transform.rotate.y < 0)transform.rotate.y += 360;
	matRot *= XMMatrixRotationY(XMConvertToRadians(transform.rotate.y));

	matTrans = XMMatrixTranslation(transform.pos.x, transform.pos.y, transform.pos.z);

	//���W�n�����[���h���W�n�̕ϊ��s��
	matSlope.r[0] = axisX;
	matSlope.r[1] = axisY;
	matSlope.r[2] = axisZ;
	matSlope.r[3] = XMVectorSet(0, 0, 0, 1);

	// ���[���h�s��̍���
	transform.mat = XMMatrixIdentity(); // �ό`�����Z�b�g
	transform.mat *= matScale * matRot * matSlope * matTrans;
}

void Model_HSM_Component::TransformMapping()
{
	// �萔�o�b�t�@�ւ̃f�[�^�]��
	XMMATRIX* matMap = nullptr;
	HRESULT result = transformBuff->Map(0, nullptr, (void**)&matMap);
	if (SUCCEEDED(result)) {
		memcpy(matMap, &transform.mat, sizeof(XMMATRIX));
		transformBuff->Unmap(0, nullptr);
	}
}

void Model_HSM_Component::MasterColorMapping()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	MasterColor* colorMap = nullptr;
	result = colorBuff->Map(0, nullptr, (void**)&colorMap);
	if (SUCCEEDED(result)) {
		memcpy(colorMap, &masterColor, sizeof(MasterColor));
		colorBuff->Unmap(0, nullptr);
	}
}

void Model_HSM_Component::SetComponentConstBuff()
{
	DirtyMapping();
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(ROOT_PARAM::MASTER_COLOR, colorBuff->GetGPUVirtualAddress());
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(ROOT_PARAM::WORLD_MAT, transformBuff->GetGPUVirtualAddress());
}
void Model_HSM_Component::DirtyMapping()
{
	if (transformDirty.Dirty())
	{
		TransformMatUpdate();
		TransformMapping();
	}
	if (masterColorDirty.Dirty())
	{
		MasterColorMapping();
	}
}
#pragma endregion

#pragma region Model_HSM_ForIncrease
void Model_HSM_ForIncrease::PreDraw()
{
	if (culling)
	{
		PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY);
	}
	else
	{
		PipelineMgr::Instance()->SetPipePine(PIPELINE_KEY_NON_CULLING);
	}
	CameraMgr::Instance()->SetCamConstBuff(ROOT_PARAM::CAMERA);

	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);
	D3D12App::Instance()->GetCmdList()->IASetIndexBuffer(&ibView);
}

void Model_HSM_ForIncrease::DrawByComponent(Model_HSM_Component& Component)
{
	if (Component.masterColor.color.w == 0.0f)return;
	Component.SetComponentConstBuff();
	Model_HSM::DrawIndexedInstanced(nullptr);
}

void Model_HSM_ForIncrease::ManyDraw(std::list<Model_HSM_Component*>* Component)
{
	PreDraw();

	for (auto itr = Component->begin(); itr != Component->end(); itr++)
	{
		(*itr)->SetComponentConstBuff();
		Model_HSM::DrawIndexedInstanced(nullptr);
	}
}

void Model_HSM_ForIncrease::ManyDraw(std::list<Individualinfo_ForIncrease*>* Info)
{
	PreDraw();

	for (auto itr = Info->begin(); itr != Info->end(); itr++)
	{
		auto info = (*itr);
		Model_HSM::DrawIndexedInstancedIndividual(*info);
	}
}

#pragma endregion

#pragma region Model_HSM_Normal

void Model_HSM_Normal::SetPipeline()
{
	if (model->culling)
	{
		PipelineMgr::Instance()->SetPipePine(Model_HSM::PIPELINE_KEY);
	}
	else
	{
		PipelineMgr::Instance()->SetPipePine(Model_HSM::PIPELINE_KEY_NON_CULLING);
	}
}

void Model_HSM_Normal::SetCamConstBuff()
{
	CameraMgr::Instance()->SetCamConstBuff(ROOT_PARAM::CAMERA);
}

Model_HSM_Normal::Model_HSM_Normal(Model_HSM* Model) :model(Model), boneGroup(&Model->skeleton)
{
	component.MasterColorMapping();
	component.TransformMapping();
}

Model_HSM_Normal::~Model_HSM_Normal()
{
	MyFunc::SafetyDelete(model);
}

void Model_HSM_Normal::Init()
{
	component.MasterColorMapping();
	component.TransformMapping();
}

void Model_HSM_Normal::Draw()
{
	SetPipeline();

	component.SetComponentConstBuff();

	SetCamConstBuff();
	//LightMgr::Instance()->SetLightConstBuff(ROOT_PARAM::DIR_LIGHT);

	SetAdditionalBuff();

	D3D12App::Instance()->GetCmdList()->IASetVertexBuffers(0, 1, &model->vbView);
	D3D12App::Instance()->GetCmdList()->IASetIndexBuffer(&model->ibView);

	model->DrawIndexedInstanced(&boneGroup);
}
#pragma endregion

#pragma region Model_HSM_Front

void Model_HSM_Front::SetPipeline()
{
	if (model->culling)
	{
		PipelineMgr::Instance()->SetPipePine(Model_HSM_Front::PIPELINE_KEY);
	}
	else
	{
		PipelineMgr::Instance()->SetPipePine(Model_HSM_Front::PIPELINE_KEY_NON_CULLING);
	}
}

void Model_HSM_Front::SetCamConstBuff()
{
	CAM.SetConstBuff(ROOT_PARAM::CAMERA);
}

#pragma region �p�C�v���C�����
const string Model_HSM_Front::PIPELINE_KEY = "MODEL_HSM_NONCAM";
const string Model_HSM_Front::PIPELINE_KEY_NON_CULLING = "MODEL_HSM_NONCAM_NONCULLING";

void Model_HSM_Front::CreatePipeline()
{
	const wchar_t* VSname = L"HSM_NonCamVertexShader.hlsl";
	const wchar_t* PSname = L"HSM_NonCamPixelShader.hlsl";
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
		//�@��
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//uv���W
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//�{�[���ԍ�
		{
			"BONE_NO",
			0,
			DXGI_FORMAT_R16G16B16A16_SINT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		//�E�F�C�g
		{
			"WEIGHT",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		////�֊s���t���O
		//{
		//	"EDGE_FLG",
		//	0,
		//	DXGI_FORMAT_R8_UINT,
		//	0,
		//	D3D12_APPEND_ALIGNED_ELEMENT,
		//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		//	0
		//}
	};

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0�@���W�X�^
	CD3DX12_DESCRIPTOR_RANGE descRangeSRVt1;
	descRangeSRVt1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1�@���W�X�^
	CD3DX12_DESCRIPTOR_RANGE descRangeSRVt2;
	descRangeSRVt2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);	//t2�@���W�X�^
	CD3DX12_DESCRIPTOR_RANGE descRangeSRVt3;
	descRangeSRVt3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);	//t3�@���W�X�^

	CD3DX12_ROOT_PARAMETER rootparams[8];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�}�X�^�[�J���[���[�h)
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�}�e���A��)
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);	//�e�N�X�`���p
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRVt1, D3D12_SHADER_VISIBILITY_ALL);	//sph
	rootparams[4].InitAsDescriptorTable(1, &descRangeSRVt2, D3D12_SHADER_VISIBILITY_ALL);	//spa
	rootparams[5].InitAsDescriptorTable(1, &descRangeSRVt3, D3D12_SHADER_VISIBILITY_ALL);	//�g�D�[���p
	rootparams[6].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(�J����)
	rootparams[7].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(���[���h�s��)
	//rootparams[8].InitAsConstantBufferView(4, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@(DirLight)

	D3D12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���J��Ԃ�
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//�c�J��Ԃ�
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//���s���J��Ԃ�
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;	//��Ԃ��Ȃ��i�j�A���X�g�l�C�o�[�j
	samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;	//�~�b�v�}�b�v�ő�l
	samplerDesc[0].MinLOD = 0.0f;	//�~�b�v�}�b�v�ŏ��l
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplerDesc[0].ShaderRegister = 0;

	samplerDesc[1] = samplerDesc[0];
	samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;	//�J��Ԃ��Ȃ�
	samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].ShaderRegister = 1;

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY,
		VSname, PSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		true, true,
		samplerDesc, _countof(samplerDesc),true);

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_NON_CULLING,
		VSname, PSname,
		input, _countof(input),
		rootparams, _countof(rootparams),
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		false, true,
		samplerDesc, _countof(samplerDesc), true);

	//PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY_WIRE,
	//	VSname, PSname,
	//	input, _countof(input),
	//	rootparams, _countof(rootparams),
	//	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	//	true, true,
	//	samplerDesc, _countof(samplerDesc));
}

#pragma endregion

#pragma endregion

//void Individualinfo_ForIncrease::Init(HSM::Skeleton* ModelBoneOperator)
//{
//	boneTransform.resize(ModelBoneOperator->boneTransform.size());
//}

//void Individualinfo_ForIncrease::TransformAffectToMat()
//{
//	for (auto itr = boneTransform.begin(); itr != boneTransform.end(); ++itr)
//	{
//		itr->BoneTransformUpdate();
//	}
//}
