#pragma once
#include<wrl.h>
#include<string.h>
using namespace std;

#include"Singleton.h"
#include"DescHeap.h"

#include<map>
#include<functional>
#include<DirectXTex.h>
using namespace DirectX;

class SRVDescMgr :public Singleton<SRVDescMgr>
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	SRVDescMgr();
	friend Singleton<SRVDescMgr>;

#pragma region �e�N�X�`�����ۑ��p�\����
	//�ǂݍ��񂾃e�N�X�`���̏��\����
	struct Texture
	{
		string filePass;			//�t�@�C���p�X
		int texHandle;							//���蓖�Ă�ꂽ�n���h���ԍ�
		ComPtr<ID3D12Resource>buff;			//�e�N�X�`�����\�[�X�̃o�b�t�@
		CD3DX12_RESOURCE_DESC texResDesc;	//�e�N�X�`���̏��i�����A�����Ȃǁj
		Texture(const string& FilePass, int Handle, CD3DX12_RESOURCE_DESC TexResDesc)
			:filePass(FilePass), texHandle(Handle), texResDesc(TexResDesc) {};
	};

	//�P�F�h��Ԃ��p�e�N�X�`��
	struct ColorTexture
	{
		int r;
		int g;
		int b;

		int texHandle;
		ComPtr<ID3D12Resource>buff;

		ColorTexture(int Handle, int R, int G, int B)
			:texHandle(Handle), r(R), g(G), b(B) {};
	};
#pragma endregion

	DescHeap heap;

	//�ǂݍ��񂾃e�N�X�`���̏��Ǘ�
	std::vector<Texture>texInfo;
	//������P�F�e�N�X�`���̏��Ǘ�
	std::vector<ColorTexture>colorTex;

	//���[�h�p�̃����_���̂��߂̌^
	using LoadLambda_t = std::function<HRESULT(const std::wstring& Path, TexMetadata* Meta, ScratchImage& Img)>;
	std::map<std::string, LoadLambda_t>loadLambdaTable;

public:
	int CreateSRV(const string& FilePass);
	int CreateSRV(int R, int G, int B);
	//GPU���\�[�X����쐬
	int CreateSRV(ID3D12Resource* Resource, DXGI_FORMAT ColorFormat);

	void SetSRV(const int& SRVHandle, const int& RootParamIndex);

	const CD3DX12_RESOURCE_DESC& GetTexDesc(const int& SRVHandle);
};