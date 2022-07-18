#include"HSMShaderHeader.hlsli"

static const min16int NONE = -1;

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD,
	min16int4 boneno : BONE_NO, float4 weight : WEIGHT)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	
    float4 resultPos = pos;
	
	//�{�[���s��
	//BDEF4(�{�[��4����3�ƁA���ꂼ��̃E�F�C�g�l�B�E�F�C�g���v��1.0�ł���ۏ�͂��Ȃ�)
    if (boneno[2] != NONE)
    {
        int num;
        
        if (boneno[3] != NONE)    //�{�[���S��
        {
            num = 4;
        }
        else //�{�[���R��
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
	//BDEF2(�{�[��2�ƁA�{�[��1�̃E�F�C�g�l(PMD����))
	else if(boneno[1] != NONE)
    {
        matrix mat = bones[boneno[0]] * weight[0];
        mat += bones[boneno[1]] * (1 - weight[0]);
        
        resultPos = mul(mat, pos);
    }
	//BDEF1(�{�[���̂�)
	else if(boneno[0]!=NONE)
    {
        resultPos = mul(bones[boneno[0]], pos);
    }
	
    float4 wpos = mul(world, resultPos); //���[���h�ϊ�

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(view, wpos); //�r���[�ϊ�

	output.depthInView = output.svpos.z;	//�J�������猩��Z

	output.svpos = mul(proj, output.svpos);	//�v���W�F�N�V�����ϊ�
	output.worldpos = wpos;
	output.normal = normal;
	output.vnormal = wnormal;
	output.uv = uv;
    output.ray = normalize(resultPos.xyz - cameraPos);

	return output;
}