#include "Material_HSMA.h"
#include"D3D12App.h"
#include"SRVDescMgr.h"

int Material_HSMA::WHITE_TEX_HANDLE = -1;
int Material_HSMA::BLACK_TEX_HANDLE = -1;
int Material_HSMA::DEFAULT_GRADATION_TEX = -1;

void Material_HSMA::CreateBuff()
{
	if (WHITE_TEX_HANDLE == -1)
	{
		WHITE_TEX_HANDLE = SRVDescMgr::Instance()->CreateSRV(255, 255, 255);
	}
	if (BLACK_TEX_HANDLE == -1)
	{
		BLACK_TEX_HANDLE = SRVDescMgr::Instance()->CreateSRV(0, 0, 0);
	}
	if (DEFAULT_GRADATION_TEX == -1)
	{
		DEFAULT_GRADATION_TEX = SRVDescMgr::Instance()->CreateSRV("resource/model/toon_def.png");
	}

	HRESULT result;
	result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(HSMA::MaterialForHLSL) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&materialBuff));

	this->ConstMapping();
}

void Material_HSMA::ConstMapping()
{
	HRESULT result;
	// 定数バッファへデータ転送
	HSMA::MaterialForHLSL* materialMap = nullptr;
	result = materialBuff->Map(0, nullptr, (void**)&materialMap);
	if (SUCCEEDED(result)) {
		memcpy(materialMap, &material.material, sizeof(HSMA::MaterialForHLSL));
		materialBuff->Unmap(0, nullptr);
	}
}

void Material_HSMA::SetMaterialConstBuff(int MaterialRootParam, int NormalRootParam, int SPHRootParam, int SPARootParam, int ToonRootParam)
{
	//マテリアルセット
	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(MaterialRootParam, materialBuff->GetGPUVirtualAddress());

	if (normalTexHandle != -1)
	{
		SRVDescMgr::Instance()->SetSRV(normalTexHandle, NormalRootParam);
	}
	else
	{
		SRVDescMgr::Instance()->SetSRV(WHITE_TEX_HANDLE, NormalRootParam);
	}
	if (sphereTexHandle != -1)
	{
		if (material.additional.sphereMode == HSMA::SPHERE_MODE::MUL)
		{
			SRVDescMgr::Instance()->SetSRV(sphereTexHandle, SPHRootParam);
			SRVDescMgr::Instance()->SetSRV(BLACK_TEX_HANDLE, SPARootParam);
		}
		else if (material.additional.sphereMode == HSMA::SPHERE_MODE::ADD)
		{
			SRVDescMgr::Instance()->SetSRV(WHITE_TEX_HANDLE, SPHRootParam);
			SRVDescMgr::Instance()->SetSRV(sphereTexHandle, SPARootParam);
		}
	}
	else
	{
		SRVDescMgr::Instance()->SetSRV(WHITE_TEX_HANDLE, SPHRootParam);
		SRVDescMgr::Instance()->SetSRV(BLACK_TEX_HANDLE, SPARootParam);
	}
	if (toonTexHandle != -1)
	{
		SRVDescMgr::Instance()->SetSRV(toonTexHandle, ToonRootParam);
	}
	else
	{
		SRVDescMgr::Instance()->SetSRV(DEFAULT_GRADATION_TEX, ToonRootParam);
	}
}