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

PSOutput main(VSOutput input)
{
	//�f�B���N�V�������C�g
	//float3 diffDirection = CalcLambertDiffuse(dirDirection,dirColor,input.normal);
	//float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, input.normal, input.ray);

	////�|�C���g���C�g
	////�T�[�t�F�C�X�ɓ��Ђ���|�C���g���C�g�̌��̌���
	//float3 ligDir = input.worldpos - ptPosition;
	//ligDir = normalize(ligDir);
	//float3 diffPoint = CalcLambertDiffuse(ligDir, ptColor, input.normal);
	//float3 specPoint = CalcPhongSpecular(ligDir, ptColor, input.normal, input.ray);
	////�����ɂ��e�������v�Z����
	////�|�C���g���C�g�Ƃ̋���
	//float3 distance = length(input.worldpos - ptPosition);
	////�e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
	//float affect = 1.0f - 1.0f / ptRange * distance;
	//if (affect < 0.0f)affect = 0.0f;
	////�e�����w���֐��I�ɂ���
	//affect = pow(affect, 3.0f);
	//diffPoint *= affect;
	//specPoint *= affect;

	//float3 diffuseLig = diffDirection + diffPoint;
	//float3 specularLig = specDirection + specPoint;

	////�g�U���ˌ��Ƌ��ʔ��ˌ��Ɗ����𑫂��Z���āA�ŏI�I�Ȍ������߂�
	//float3 lig = diffuseLig + specularLig + ambientLight;
	////�e�N�X�`���J���[�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
	//float4 finalColor = tex.Sample(smp, input.uv);
	//finalColor.xyz *= lig;

	//return finalColor;

//�o�b�N�A�b�v
	//���̌������x�N�g���i���s�����j
	//float3 light = normalize(float3(0,0,1));
	//float3 light = normalize(float3(0,0,1));
	//���C�g�̃J���[�i���j
	//float3 lightColor = float3(1, 1, 1);
	////�f�B�q���[�Y�v�Z
	//float diffuseB = saturate(dot(-light, input.normal));
	//float4 toonDif = toon.Sample(smpToon, float2(0, 1.0 - diffuseB));
	////���̔��˃x�N�g��
	//float3 refLight = normalize(reflect(light, input.normal.xyz));
	//float specularB = pow(saturate(dot(refLight, -input.ray)), specular_c);
	////�X�t�B�A�}�b�v�pUV
	//float2 sphereMapUV = input.vnormal.xy;
	//sphereMapUV = (sphereMapUV + float2(1, -1)) * float2(0.5, -0.5);
	////�e�N�X�`���J���[
	//float4 texColor = tex.Sample(smp, input.uv);

	//return max(saturate(toonDif //�P�x�i�g�D�[���j
	//	* diffuse //�f�B�q���[�Y�F
	//	* texColor	//�e�N�X�`���J���[
	//	* sph.Sample(smp, sphereMapUV))	//�X�t�B�A�}�b�v�i��Z�j
	//	+ saturate(spa.Sample(smp, sphereMapUV) * texColor	//�X�t�B�A�}�b�v�i���Z�j
	//	+ float4(specularB * specular.rgb, 1)),	//�X�y�L����
	//	float4(texColor * ambient, 1));//�A���r�G���g

    float4 texColor = tex.Sample(smp, input.uv);
    texColor.w *= alpha;
    texColor *= masterColor;

    if (shadow)
    {
		//float3 light = normalize(float3(0,0,1));
        float3 light = normalize(float3(1, 3.5f, 1));
		//���C�g�̃J���[�i���j
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

