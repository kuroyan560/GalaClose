#include"HSMShaderHeader.hlsli"
Texture2D<float4>tex:register(t0);	//０番スロットに設定されたテクスチャ
Texture2D<float4>sph:register(t1);
Texture2D<float4>spa:register(t2);
Texture2D<float4>toon:register(t3);
SamplerState smp : register(s0);	//０番スロットに設定されたサンプラー
SamplerState smpToon : register(s1);

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
	//ディレクションライト
	//float3 diffDirection = CalcLambertDiffuse(dirDirection,dirColor,input.normal);
	//float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, input.normal, input.ray);

	////ポイントライト
	////サーフェイスに入社するポイントライトの光の向き
	//float3 ligDir = input.worldpos - ptPosition;
	//ligDir = normalize(ligDir);
	//float3 diffPoint = CalcLambertDiffuse(ligDir, ptColor, input.normal);
	//float3 specPoint = CalcPhongSpecular(ligDir, ptColor, input.normal, input.ray);
	////距離による影響率を計算する
	////ポイントライトとの距離
	//float3 distance = length(input.worldpos - ptPosition);
	////影響率は距離に比例して小さくなっていく
	//float affect = 1.0f - 1.0f / ptRange * distance;
	//if (affect < 0.0f)affect = 0.0f;
	////影響を指数関数的にする
	//affect = pow(affect, 3.0f);
	//diffPoint *= affect;
	//specPoint *= affect;

	//float3 diffuseLig = diffDirection + diffPoint;
	//float3 specularLig = specDirection + specPoint;

	////拡散反射光と鏡面反射光と環境光を足し算して、最終的な光を求める
	//float3 lig = diffuseLig + specularLig + ambientLight;
	////テクスチャカラーに求めた光を乗算して最終出力カラーを求める
	//float4 finalColor = tex.Sample(smp, input.uv);
	//finalColor.xyz *= lig;

	//return finalColor;

//バックアップ
	//光の向かうベクトル（平行光線）
	//float3 light = normalize(float3(0,0,1));
	//float3 light = normalize(float3(0,0,1));
	//ライトのカラー（白）
	//float3 lightColor = float3(1, 1, 1);
	////ディヒューズ計算
	//float diffuseB = saturate(dot(-light, input.normal));
	//float4 toonDif = toon.Sample(smpToon, float2(0, 1.0 - diffuseB));
	////光の反射ベクトル
	//float3 refLight = normalize(reflect(light, input.normal.xyz));
	//float specularB = pow(saturate(dot(refLight, -input.ray)), specular_c);
	////スフィアマップ用UV
	//float2 sphereMapUV = input.vnormal.xy;
	//sphereMapUV = (sphereMapUV + float2(1, -1)) * float2(0.5, -0.5);
	////テクスチャカラー
	//float4 texColor = tex.Sample(smp, input.uv);

	//return max(saturate(toonDif //輝度（トゥーン）
	//	* diffuse //ディヒューズ色
	//	* texColor	//テクスチャカラー
	//	* sph.Sample(smp, sphereMapUV))	//スフィアマップ（乗算）
	//	+ saturate(spa.Sample(smp, sphereMapUV) * texColor	//スフィアマップ（加算）
	//	+ float4(specularB * specular.rgb, 1)),	//スペキュラ
	//	float4(texColor * ambient, 1));//アンビエント

    float4 texColor = tex.Sample(smp, input.uv);
    texColor.w *= alpha;
    texColor *= masterColor;

    if (shadow)
    {
		//float3 light = normalize(float3(0,0,1));
        float3 light = normalize(float3(1, 3.5f, 1));
		//ライトのカラー（白）
        float3 lightColor = float3(1, 1, 1);
		
  //      float3 diffuse = CalcLambertDiffuse(light, lightColor, input.normal);
  //      float3 specular = CalcPhongSpecular(light, lightColor, input.normal, input.ray);
    
		//texColor.xyz *= (diffuse + specular);
		
        float p = dot(input.normal * -1.0f, light.xyz);
        p = p * 0.5f + 0.5f;
        p = p * p;
    
        float3 toon;
        if (p < 0.2f)
        {
            toon = (0.85f, 0.85f, 0.85f);
        }
		else if(p < 0.4f)
        {
            toon = (0.9f, 0.9f, 0.9f);
        }
        else if (p < 0.6f)
        {
            toon = (1.0f, 1.0f, 1.0f);
        }
		else if(p < 0.8f)
        {
            toon = (1.1f, 1.1f, 1.1f);
        }
        else
        {
            toon = (1.15f, 1.15f, 1.15f);
        }
    
        texColor.xyz *= toon.xyz;
    }

	PSOutput output;
	
    if (flash)
    {
        texColor.x += (1.0f - texColor.x) * flash;
        texColor.y += (1.0f - texColor.y) * flash;
        texColor.z += (1.0f - texColor.z) * flash;
    }
	
	output.color = texColor;
	output.depth = input.depthInView;
	
    output.clip = 1;
    if (output.color.a == 0)
    {
        output.clip = 0;
    }

	return output;
}

