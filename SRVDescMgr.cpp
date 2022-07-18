#include "SRVDescMgr.h"
using namespace DirectX;

#include"D3D12App.h"
#include"WinApp.h"

#include<algorithm>

#include"MyFunc.h"

SRVDescMgr::SRVDescMgr()
	:heap(CBV_SRV_UAV, D3D12App::Instance()->GetDev())
{
	loadLambdaTable["sph"]
		= loadLambdaTable["spa"]
		= loadLambdaTable["bmp"]
		= loadLambdaTable["png"]
		= loadLambdaTable["jpg"]
		= [](const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)->HRESULT
	{
		return LoadFromWICFile(Path.c_str(), WIC_FLAGS_NONE, Meta, Img);
	};
	loadLambdaTable["tga"]
		= [](const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)->HRESULT
	{
		return LoadFromTGAFile(Path.c_str(), Meta, Img);
	};
	loadLambdaTable["dds"]
		= [](const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)->HRESULT
	{
		return LoadFromDDSFile(Path.c_str(), DDS_FLAGS_NONE, Meta, Img);
	};
}

int SRVDescMgr::CreateSRV(const string& FilePass)
{
	//���Ƀ��[�h����������
	if (!texInfo.empty())
	{
		for (auto itr = texInfo.begin(); itr != texInfo.end(); itr++)
		{
			if (itr->filePass == FilePass)
			{
				return itr->texHandle;
			}
		}
	}

	HRESULT result;
	/*
		WIC�e�N�X�`���̃��[�h
		metadata�ɂ͉摜�T�C�Y�ȂǁA�e�N�X�`���Ƃ��Ă̊e���񂪓���A
		img�|�C���^�Ƀr�b�g�}�b�v�`���ł̉摜�f�[�^���̂��̂̃A�h���X������
	*/
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	auto wtexpath = MyFunc::GetWideStrFromStr(FilePass);
	auto ext = MyFunc::GetExtension(FilePass);

	result = loadLambdaTable[ext](
		wtexpath,
		&metadata,
		scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0);	//���f�[�^���o

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"�摜�̃f�[�^���o�Ɏ��s");
		return -1;
	}

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,	//RGBA�t�H�[�}�b�g
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���p���\�[�X�o�b�t�@�̐���
	texInfo.push_back(Texture(FilePass, heap.GetDescNum(), texresDesc));
	CD3DX12_HEAP_PROPERTIES heapPropForTex(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&heapPropForTex,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texInfo.back().buff));

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"�摜���\�[�X�o�b�t�@�����Ɏ��s");
		return -1;
	}

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texInfo.back().buff->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		img->pixels,	//���f�[�^�A�h���X
		(UINT)img->rowPitch,	//1���C���T�C�Y
		(UINT)img->slicePitch	//�P���T�C�Y
	);

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"�摜�̃��\�[�X�o�b�t�@�]���Ɏ��s");
		return -1;
	}

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v�ɃV�F�[�_���\�[�X�r���[�쐬
	D3D12App::Instance()->GetDev()->CreateShaderResourceView(
		texInfo.back().buff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,				//�e�N�X�`���ݒ���
		heap.GetCpuHandle());

	heap.AddDescNum();

	return heap.GetDescNum() - 1;
}

int SRVDescMgr::CreateSRV(int R, int G, int B)
{
	//���Ƀ��[�h����������
	if (!colorTex.empty())
	{
		for (auto itr = colorTex.begin(); itr != colorTex.end(); itr++)
		{
			if (itr->r == R && itr->g == G && itr->b == B)
			{
				return itr->texHandle;
			}
		}
	}

	HRESULT result;

	const int texWidth = 256;
	const int texDataCount = texWidth * texWidth;
	//�e�N�X�`���f�[�^�z��
	XMFLOAT4* texturedata = new XMFLOAT4[texDataCount];

	//�S�s�N�Z���̐F��������
	for (int i = 0; i < texDataCount; i++)
	{
		texturedata[i].x = (float)R / 255.0f;	//R
		texturedata[i].y = (float)G / 255.0f;	//G
		texturedata[i].z = (float)B / 255.0f;	//B
		texturedata[i].w = 1.0f;	//A
	}

	D3D12_HEAP_PROPERTIES texHeapProp{};	//�e�N�X�`���q�[�v�ݒ�
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//�e�N�X�`�����\�[�X�ݒ�
	D3D12_RESOURCE_DESC texresDesc{};	//���\�[�X�ݒ�
	texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2D�e�N�X�`���p
	texresDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//RGBA�t�H�[�}�b�g
	texresDesc.Width = texWidth;
	texresDesc.Height = texWidth;
	texresDesc.DepthOrArraySize = 1;
	texresDesc.MipLevels = 1;
	texresDesc.SampleDesc.Count = 1;

	//�e�N�X�`���p���\�[�X�o�b�t�@�̐���
	colorTex.push_back(ColorTexture(heap.GetDescNum(), R, G, B));

	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&colorTex.back().buff));

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"�P�F�h��Ԃ����\�[�X�o�b�t�@�̐����Ɏ��s");
		return -1;
	}

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = colorTex.back().buff->WriteToSubresource(
		0,
		nullptr,	//�S�̈�փR�s�[
		texturedata,	//���f�[�^�A�h���X
		sizeof(XMFLOAT4) * texWidth,	//1���C���T�C�Y
		sizeof(XMFLOAT4) * texDataCount	//�P���T�C�Y
	);

	delete[] texturedata;

	if (result != S_OK)
	{
		WinApp::Instance()->DebugError(L"�P�F�h��Ԃ����\�[�X�o�b�t�@�̓]���Ɏ��s");
		return -1;
	}

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v�ɃV�F�[�_���\�[�X�r���[�쐬
	D3D12App::Instance()->GetDev()->CreateShaderResourceView(
		colorTex.back().buff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,				//�e�N�X�`���ݒ���
		heap.GetCpuHandle());

	heap.AddDescNum();

	return heap.GetDescNum() - 1;
}

int SRVDescMgr::CreateSRV(ID3D12Resource* Resource, DXGI_FORMAT ColorFormat)
{
	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = ColorFormat;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v�ɃV�F�[�_���\�[�X�r���[�쐬
	D3D12App::Instance()->GetDev()->CreateShaderResourceView(
		Resource,	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,				//�e�N�X�`���ݒ���
		heap.GetCpuHandle());

	heap.AddDescNum();

	return heap.GetDescNum() - 1;
}

void SRVDescMgr::SetSRV(const int& SRVHandle, const int& RootParamIndex)
{
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootDescriptorTable(RootParamIndex, heap.GetGpuHandle(SRVHandle));
}

const CD3DX12_RESOURCE_DESC& SRVDescMgr::GetTexDesc(const int& SRVHandle)
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	auto result = std::find_if(texInfo.begin(), texInfo.end(), [SRVHandle](Texture Tex) { return Tex.texHandle == SRVHandle; });
	return result->texResDesc;
}
