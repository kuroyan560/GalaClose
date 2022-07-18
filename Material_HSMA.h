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
		float specular_c = 0.0f;	//スペキュラー係数( c は coefficient の頭文字)
		float3 ambient = { 0,0,0 };
		float alpha = 1.0f;
	};
	static const enum SPHERE_MODE { NONE, MUL, ADD };
	struct AdditionalMaterial
	{
		unsigned char bitFlag;	//描画フラグ(8bit) - 各bit 0:OFF 1:ON
	/*	0x01:両面描画, 0x02:地面影, 0x04:セルフシャドウマップへの描画, 0x08:セルフシャドウの描画, 0x10:エッジ描画 */
		float4 edgeColor;
		float edgeSize;
		//スフィアモード 0:無効 1:乗算(sph) 2:加算(spa) 3:サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
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