#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

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

PSOutput main(VSOutput input)
{
	//// �e�N�X�`���}�b�s���O
	//float4 texcolor = tex.Sample(smp, input.uv);

	//// ����x
	//const float shininess = 4.0f;
	//// ���_���王�_�ւ̕����x�N�g��
	//float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//// �����ˌ�
	//float3 ambient = m_ambient;

	//// �V�F�[�f�B���O�ɂ��F
	//float4 shadecolor = float4(ambient, m_alpha);

	//for (int i = 0; i < DIRLIGHT_NUM; i++) {
	//	if (dirLights[i].active) {
	//		// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
	//		// ���ˌ��x�N�g��
	//		float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
	//		// �g�U���ˌ�
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		// ���ʔ��ˌ�
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//		// �S�ĉ��Z����
	//		shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
	//	}
	//}

	//// �V�F�[�f�B���O�ɂ��F�ŕ`��
	//return shadecolor * texcolor;

	float3 light = normalize(float3(0,-1,0));
	//���C�g�̃J���[�i���j
	float3 lightColor = float3(1, 1, 1);
	////�f�B�q���[�Y�v�Z
	//float diffuseB = saturate(dot(-light, input.normal));
	////���̔��˃x�N�g��
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