#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float3 CalcLambertDiffuse(float3 DirDirection, float3 DirColor, float3 Normal)
{
	float t = dot(Normal, DirDirection);
	t *= -1.0f;
	if (t < 0.0f)t = 0.0f;

	//拡散反射光
	return DirColor * t;
}

float3 CalcPhongSpecular(float3 DirDirection, float3 DirColor, float3 Normal, float3 Ray)
{
	//反射ベクトル
	float3 refVec = reflect(DirDirection, Normal);

	//鏡面反射の強さ
	float t = dot(refVec, Ray);

	//鏡面反射の強さを絞る
	t = pow(t, 5.0f);

	//鏡面反射光
	return DirColor * t;
}

PSOutput main(VSOutput input)
{
	//// テクスチャマッピング
	//float4 texcolor = tex.Sample(smp, input.uv);

	//// 光沢度
	//const float shininess = 4.0f;
	//// 頂点から視点への方向ベクトル
	//float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//// 環境反射光
	//float3 ambient = m_ambient;

	//// シェーディングによる色
	//float4 shadecolor = float4(ambient, m_alpha);

	//for (int i = 0; i < DIRLIGHT_NUM; i++) {
	//	if (dirLights[i].active) {
	//		// ライトに向かうベクトルと法線の内積
	//		float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
	//		// 反射光ベクトル
	//		float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
	//		// 拡散反射光
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		// 鏡面反射光
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//		// 全て加算する
	//		shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
	//	}
	//}

	//// シェーディングによる色で描画
	//return shadecolor * texcolor;

	float3 light = normalize(float3(0,-1,0));
	//ライトのカラー（白）
	float3 lightColor = float3(1, 1, 1);
	////ディヒューズ計算
	//float diffuseB = saturate(dot(-light, input.normal));
	////光の反射ベクトル
	//float3 refLight = normalize(reflect(light, input.normal.xyz));
	//float specularB = pow(saturate(dot(refLight, -input.ray)), 1.0f);

	//float4 texcolor = tex.Sample(smp, input.uv);

	//float4 a = saturate((m_diffuse, 1) * texcolor + float4(specularB * m_specular.rgb, 1));

	//return float4(max(a.rgb, texcolor * m_ambient), m_alpha);

	//float3 diffuse = CalcLambertDiffuse(light, lightColor, input.normal);
	//float3 specular = CalcPhongSpecular(light, lightColor, input.normal, input.ray);
	float4 texColor = tex.Sample(smp, input.uv);
	//float3 ambient = (float3(0.2f, 0.2f, 0.2f));
	//texColor.xyz *= (diffuse + specular + ambient);
	texColor.x *= m_alpha;
	texColor.y *= m_alpha;
	texColor.z *= m_alpha;
	texColor.w = m_alpha;

    PSOutput output;
    output.color = texColor;
    output.depth = input.depthInView;
	
    output.clip = 1;
    if (output.color.a == 0)
    {
        output.clip = 0;
    }

	return output;
}