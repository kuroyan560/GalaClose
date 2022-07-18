#pragma once
#include<string>
#include<DirectXMath.h>
using namespace DirectX;

#include<wrl.h>
#include<d3d12.h>

class Material
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct Const
	{
		XMFLOAT3 ambient; // �A���r�G���g�W��
		float pad1; // �p�f�B���O
		XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
		float pad2; // �p�f�B���O
		XMFLOAT3 specular; // �X�y�L�����[�W��
		float alpha;	// �A���t�@
	};
public:
	static Material* Create();

private:
	Material()
	{
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

public:
	std::string name;	// �}�e���A����
	XMFLOAT3 ambient;	// �A���r�G���g�e���x
	XMFLOAT3 diffuse;	// �f�B�t���[�Y�e���x
	XMFLOAT3 specular;	// �X�y�L�����[�e���x
	float alpha;		// �A���t�@
	int texHandle = -1;

	void ConstMapping();
	void Set(int ConstParamIndex, int SRVParamIndex);
};