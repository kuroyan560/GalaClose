#include "Material.h"
#include"D3D12App.h"
#include"SRVDescMgr.h"

Material* Material::Create()
{
	Material* instance = new Material;

	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Material::Const) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&instance->constBuff));

	if (FAILED(result)) {
		assert(0);
	}

	return instance;
}

void Material::ConstMapping()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	Material::Const* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambient = ambient;
		constMap->diffuse = diffuse;
		constMap->specular = specular;
		constMap->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}
}

void Material::Set(int ConstParamIndex, int SRVParamIndex)
{
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	SRVDescMgr::Instance()->SetSRV(texHandle, SRVParamIndex);

	// �}�e���A���̒萔�o�b�t�@���Z�b�g
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(ConstParamIndex, constBuff->GetGPUVirtualAddress());
}
