#include"HSMShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD,
	min16int4 boneno : BONE_NO, float4 weight : WEIGHT)
{
	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mul(mul(proj, view), world), pos);
	output.worldpos = mul(world, pos);
	output.normal = normal;
	output.vnormal = wnormal;
	output.uv = uv;
	output.ray = normalize(pos.xyz - cameraPos);

	return output;
}