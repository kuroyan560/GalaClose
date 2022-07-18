#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	VSOutput output; // ピクセルシェーダーに渡す値
    output.svpos = mul(view, wpos); //ビュー変換
	
    output.depthInView = output.svpos.z; //カメラから見たZ

    output.svpos = mul(proj, output.svpos); //プロジェクション変換
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;
	output.ray = normalize(pos.xyz - cameraPos);

	return output;
}