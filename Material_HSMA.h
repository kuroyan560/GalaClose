#pragma once
#include<wrl.h>
#include<string>
using namespace std;
#include"MyFunc.h"
using namespace MyDef;
#include<d3d12.h>
using namespace DirectX;

namespace HSMA
{
	struct Header
	{
		string name;
	};

	struct MaterialForHLSL
	{
		float4 diffuse = { 0,0,0,1 };
		float3 specular = { 0,0,0 };
		float specular_c = 0.0f;	//�X�y�L�����[�W��( c �� coefficient �̓�����)
		float3 ambient = { 0,0,0 };
		float alpha = 1.0f;
	};
	static const enum SPHERE_MODE { NONE, MUL, ADD };
	struct AdditionalMaterial
	{
		unsigned char bitFlag;	//�`��t���O(8bit) - �ebit 0:OFF 1:ON
	/*	0x01:���ʕ`��, 0x02:�n�ʉe, 0x04:�Z���t�V���h�E�}�b�v�ւ̕`��, 0x08:�Z���t�V���h�E�̕`��, 0x10:�G�b�W�`�� */
		float4 edgeColor;
		float edgeSize;
		//�X�t�B�A���[�h 0:���� 1:��Z(sph) 2:���Z(spa) 3:�T�u�e�N�X�`��(�ǉ�UV1��x,y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
		unsigned char sphereMode;
	};
	struct Material
	{
		MaterialForHLSL material;
		AdditionalMaterial additional;
	};
}
class Material_HSMA
{
private:
	friend class PMXLoader;
	friend class FBXLoader;
	friend class Model_HSM;
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static int WHITE_TEX_HANDLE;
	static int BLACK_TEX_HANDLE;
	static int DEFAULT_GRADATION_TEX;
private:
	HSMA::Header head;

	HSMA::Material material;
	ComPtr<ID3D12Resource> materialBuff = nullptr;

	int normalTexHandle = -1;
	int sphereTexHandle = -1;
	int toonTexHandle = -1;

	void CreateBuff();
	void ConstMapping();
public:
	void SetMaterialConstBuff(int MaterialRootParam, int NormalRootParam, int SPHRootParam, int SPARootParam, int ToonRootParam);
};