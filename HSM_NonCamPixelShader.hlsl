#include"HSMShaderHeader.hlsli"
Texture2D<float4>tex:register(t0);	//�O�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4>sph:register(t1);
Texture2D<float4>spa:register(t2);
Texture2D<float4>toon:register(t3);
SamplerState smp : register(s0);	//�O�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smpToon : register(s1);

float3 CalcLambertDiffuse(float3 DirDirection, float3 DirColor, float3 Normal)
{
	float t = dot(Normal, DirDirection);
	t *= -1.0f;
	if (t < 0.0f)t = 0.0f;

	//�g�U���ˌ�
	return DirColor * t;
}

float3 CalcPhongSpecular(float3 DirDirection, float3 DirColor, float3 Normal, float3 Ray)
{
	//���˃x�N�g��
	float3 refVec = reflect(DirDirection, Normal);

	//���ʔ��˂̋���
	float t = dot(refVec, Ray);

	//���ʔ��˂̋������i��
	t = pow(t, 5.0f);

	//���ʔ��ˌ�
	return DirColor * t;
}

float4 main(VSOutput input) : SV_TARGET
{
	////���̌������x�N�g���i���s�����j
	//float3 light = normalize(float3(-0.5,-1,1));
	////���C�g�̃J���[�i���j
	//float3 lightColor = float3(1, 1, 1);

	//float3 diffuse0 = CalcLambertDiffuse(light, lightColor, input.vnormal);
	//float3 specular0 = CalcPhongSpecular(light, lightColor, input.vnormal, input.ray);
	float4 texColor = tex.Sample(smp, input.uv);
	texColor.w *= alpha;
    texColor *= masterColor;
	//texColor.xyz *= (diffuse0 + diffuse.xyz + specular0 + specular + ambient);

	return texColor;
}