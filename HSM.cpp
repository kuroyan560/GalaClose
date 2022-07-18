#include"HSM.h"
#include"D3D12App.h"

HSM::BoneTransform::BoneTransform()
{
	dirty.AddInspector(&pos.x);
	dirty.AddInspector(&pos.y);
	dirty.AddInspector(&pos.z);
	dirty.AddInspector(&scale.x);
	dirty.AddInspector(&scale.y);
	dirty.AddInspector(&scale.z);
	dirty.AddInspector(&rotate.x);
	dirty.AddInspector(&rotate.y);
	dirty.AddInspector(&rotate.z);
}

bool HSM::BoneTransform::TransformAffectToMat()
{
	if (dirty.Dirty())
	{
		HRESULT result;

		XMMATRIX matScale, matRot, matTrans;
		// �X�P�[���A��]�A���s�ړ��s��̌v�Z
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotate.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotate.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotate.y));
		matTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);

		// ���[���h�s��̍���
		*mat = XMMatrixIdentity(); // �ό`�����Z�b�g
		*mat *= matScale * matRot * matTrans;
		return true;
	}
	return false;
}

HSM::BoneGroup::BoneGroup(Skeleton* Skeleton)
	:skeleton(Skeleton)
{
	if (Skeleton != nullptr)
	{
		if (!Skeleton->bones.empty())
		{
			HRESULT result;
			//�{�[���s���GPU���\�[�X
			UINT sizeBB = static_cast<UINT>(sizeof(XMMATRIX) * Skeleton->bones.size());
			// ���_�o�b�t�@����
			result = D3D12App::Instance()->GetDev()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(sizeBB),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&boneBuff));

			//Skeleton����{�[���̐���ǂݎ���āABoneTransform�ƍs��𐶐����������֘A�t��
			boneTransform.resize(Skeleton->bones.size());
			boneMatrices.resize(Skeleton->bones.size());
			for (int i = 0; i < Skeleton->bones.size(); ++i)
			{
				boneTransform[i].mat = &boneMatrices[i];
			}
			std::fill(boneMatrices.begin(), boneMatrices.end(), XMMatrixIdentity());

			XMMATRIX* boneMap = nullptr;
			result = boneBuff->Map(0, nullptr, (void**)&boneMap);
			if (SUCCEEDED(result))
			{
				copy(boneMatrices.begin(), boneMatrices.end(), boneMap);
				boneBuff->Unmap(0, nullptr);
			}
		}
	}
}

void HSM::BoneGroup::RecursiveMatrixMultiply(BoneNode* Node, const XMMATRIX& Mat)
{
	auto boneTrans = boneTransform[Node->boneIdx];

	if (boneTrans.dirty.Dirty())
	{
		boneTrans.TransformAffectToMat();
	}

	*boneTrans.mat *= Mat;

	for (auto& cnode : Node->children)
	{
		RecursiveMatrixMultiply(cnode, boneMatrices[Node->boneIdx]);
	}
}

void HSM::BoneGroup::SetBoneConstBuff(Skeleton* ModelSkeleton, int RootParam)
{
	if (this->skeleton == nullptr || this->skeleton != ModelSkeleton || this->skeleton->bones.empty())return;

	//RecursiveMatrixMultiply
	bool changed = false;
	for (auto itr = boneTransform.begin(); itr != boneTransform.end(); ++itr)
	{
		if (itr->TransformAffectToMat() && !changed)
		{
			changed = true;
		}
	}
	//�}�b�s���O
	if (changed || nowAnimation)
	{
		XMMATRIX* boneMap = nullptr;
		auto result = boneBuff->Map(0, nullptr, (void**)&boneMap);
		if (SUCCEEDED(result))
		{
			copy(boneMatrices.begin(), boneMatrices.end(), boneMap);
			boneBuff->Unmap(0, nullptr);
		}
	}

	D3D12App::Instance()->GetCmdList()->SetGraphicsRootConstantBufferView(
		RootParam, boneBuff->GetGPUVirtualAddress());
}

int HSM::Skeleton::GetIndex(std::string BoneName)
{
	int i = 0;
	auto itr = bones.begin();
	for (; itr != bones.end(); ++itr)
	{
		if (itr->name == BoneName)
		{
			break;
		}
		++i;
	}
	if (itr == bones.end())
	{
		printf("==================================\n");
		printf("���݂��Ȃ��{�[�����Q�Ƃ���܂����B�C���f�b�N�X�ԍ���-1��Ԃ��܂��B\n");
		printf("�E�Q�Ƃ��悤�Ƃ����{�[���̖��O�F%s\n", BoneName.c_str());
		return -1;
	}

	return boneNodeTable[BoneName].boneIdx;
}