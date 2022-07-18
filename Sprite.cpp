#include "Sprite.h"
#include "d3dx12.h"

#include"PipelineMgr.h"
#include"D3D12App.h"
#include"WinApp.h"

#include"SRVDescMgr.h"

#include"MyFunc.h"

int Sprite::MOUSE_IMG_ID = -1;
std::vector<Sprite*>Sprite::STATIC_SPRITES;

#pragma region �p�C�v���C�����i�ÓI�����o�j

const std::string Sprite_Normal::PIPELINE_KEY = "SPRITE";

void Sprite::CreatePipeline()
{
	const wchar_t* VSname = L"SpriteVertexShader.hlsl";
	const wchar_t* PSname = L"SpritePixelShader.hlsl";
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
	};

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0�@���W�X�^

	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);	//�萔�o�b�t�@�r���[�Ƃ��ď�����(b0)
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);	//�e�N�X�`���p

	PipelineMgr::Instance()->AddPipeLine(PIPELINE_KEY, VSname, PSname,
		input, _countof(input), rootparams, _countof(rootparams), D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, false, false);
}
#pragma endregion

#pragma region �X�v���C�g���N���X
Sprite* Sprite::HitCheckAll(XMFLOAT2 ScreenPos, int StartID)
{
	int num = -1;	//�T��������
	int nowID;	//�����蔻������ID
	if (StartID < 0 || STATIC_SPRITES.size() <= StartID)
	{
		nowID = -1;	//ID�̎w�肪�s����������擪������`�T��
	}
	else
	{
		nowID = StartID;	//�w�肳�ꂽID�̎�����T�����n�߂�
	}

	while (num != STATIC_SPRITES.size() - 1)	//�S���T���I���܂�
	{
		num++;
		nowID++;	//����ID�̃X�v���C�g���Z�b�g
		if (nowID == STATIC_SPRITES.size())nowID = 0;	//���[�܂ł�����擪�ɖ߂�

		if (nowID == MOUSE_IMG_ID)continue;		//�}�E�X�J�[�\���p�̃X�v���C�g�Ȃ疳��
		if (!STATIC_SPRITES[nowID]->draw)continue;	//�`�悳��Ă��Ȃ����̂Ȃ疳��

		if (STATIC_SPRITES[nowID]->HitCheck(ScreenPos))
		{
			return STATIC_SPRITES[nowID];	//�����蔻�肪�Ƃꂽ�炻���Ԃ�
		}
	}

	return nullptr;	//�����q�b�g���Ȃ����nullptr��Ԃ�
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

	if (flipX)	//���E���]
	{
		left *= -1;
		right *= -1;
	}
	if (flipY)	//�㉺���]
	{
		top *= -1;
		bottom *= -1;
	}

	vertices[LB].pos = { left,bottom,0.0f };		//����
	vertices[LT].pos = { left,top,0.0f };			//����
	vertices[RB].pos = { right,bottom,0.0f };		//�E��
	vertices[RT].pos = { right,top,0.0f };			//�E��

	//���_�o�b�t�@�ւ̃f�[�^�]��
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
	vertices[0] = { {},{0.0f,1.0f} };	//����
	vertices[1] = { {},{0.0f,0.0f} };	//����
	vertices[2] = { {},{1.0f,1.0f} };	//�E��
	vertices[3] = { {},{1.0f,0.0f} };	//�E��

	HRESULT result;

	//���_�o�b�t�@����
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC(CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices))),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	VertMapping();

	//���_�o�b�t�@�r���[�̍쐬
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

	vertices[0] = { {},{0.0f,1.0f} };	//����
	vertices[1] = { {},{0.0f,0.0f} };	//����
	vertices[2] = { {},{1.0f,1.0f} };	//�E��
	vertices[3] = { {},{1.0f,0.0f} };	//�E��

	HRESULT result;

	//���_�o�b�t�@����
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC(CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices))),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	VertMapping();

	//���_�o�b�t�@�r���[�̍쐬
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

	if (flipX)	//���E���]
	{
		left *= -1;
		right *= -1;
	}
	if (flipY)	//�㉺���]
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

	vertices[LB].uv = { texL,texB };	//����
	vertices[LT].uv = { texL,texT };	//����
	vertices[RB].uv = { texR,texB };	//�E��
	vertices[RT].uv = { texR,texT };	//�E��

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

#pragma region �X�v���C�g�̃R���|�[�l���g
Sprite_Component::Sprite_Component()
{
	HRESULT result;
	//��`�o�b�t�@�̐���
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC(CD3DX12_RESOURCE_DESC::Buffer((sizeof(constBuff) + 0xff) & ~0xff)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	//�萔�o�b�t�@�Ƀf�[�^�]��
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
	//�萔�o�b�t�@�Ƀf�[�^�]��
	SPRITE::Const* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;	//�F�w��iRGBA�j
	constBuff->Unmap(0, nullptr);
}

void Sprite_Component::MatMapping()
{
	//���[���h�s��̍X�V
	transform.mat = XMMatrixIdentity();
	transform.mat = XMMatrixScaling(transform.scale.x, transform.scale.y, 1.0f);
	transform.mat *= XMMatrixRotationZ(XMConvertToRadians(transform.rotate.x));
	transform.mat *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	transform.mat *= XMMatrixRotationY(XMConvertToRadians(transform.rotate.y));
	transform.mat *= XMMatrixTranslation(transform.pos.x, transform.pos.y, 0);

	//�萔�o�b�t�@�Ƀf�[�^�]��
	SPRITE::Const* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = transform.mat * WinApp::PARALLEL_MAT_PROJECTION;	//�s��̍���
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

#pragma region �ʏ�̃X�v���C�g
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

#pragma region �摜�����X�v���C�g
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