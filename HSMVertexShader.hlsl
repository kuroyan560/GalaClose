#include"HSMShaderHeader.hlsli"

static const min16int NONE = -1;

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD,
	min16int4 boneno : BONE_NO, float4 weight : WEIGHT)
{
	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	
    float4 resultPos = pos;
	
	//ボーン行列
	//BDEF4(ボーン4つ又は3つと、それぞれのウェイト値。ウェイト合計が1.0である保障はしない)
    if (boneno[2] != NONE)
    {
        int num;
        
        if (boneno[3] != NONE)    //ボーン４つ
        {
            num = 4;
        }
        else //ボーン３つ
        {
            num = 3;
        }
        
        matrix mat = bones[boneno[0]] * weight[0];
        for (int i = 1; i < num; ++i)
        {
            mat += bones[boneno[i]] * weight[i];
        }
        resultPos = mul(mat, pos);
    }
	//BDEF2(ボーン2つと、ボーン1のウェイト値(PMD方式))
	else if(boneno[1] != NONE)
    {
        matrix mat = bones[boneno[0]] * weight[0];
        mat += bones[boneno[1]] * (1 - weight[0]);
        
        resultPos = mul(mat, pos);
    }
	//BDEF1(ボーンのみ)
	else if(boneno[0]!=NONE)
    {
        resultPos = mul(bones[boneno[0]], pos);
    }
	
    float4 wpos = mul(world, resultPos); //ワールド変換

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(view, wpos); //ビュー変換

	output.depthInView = output.svpos.z;	//カメラから見たZ

	output.svpos = mul(proj, output.svpos);	//プロジェクション変換
	output.worldpos = wpos;
	output.normal = normal;
	output.vnormal = wnormal;
	output.uv = uv;
    output.ray = normalize(resultPos.xyz - cameraPos);

	return output;
}