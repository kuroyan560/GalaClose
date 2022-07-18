#include"ParticleShaderHeader.hlsli"

static const uint vnum = 4; //�l�p�`

//�Z���^�[����̃I�t�Z�b�g
static const float4 offsetArray[vnum] =
{
	float4(-0.5f,-0.5f,0,0),	//����
	float4(-0.5f,+0.5f,0,0),	//����
	float4(+0.5f,-0.5f,0,0),	//�E��
	float4(+0.5f,+0.5f,0,0),	//�E��
};

static const float2 uvArray[vnum] =
{
	float2(0.0f,1.0f),//����
	float2(0.0f,0.0f),//����
	float2(1.0f,1.0f),//�E��
	float2(1.0f,0.0f) //�E��
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : POSITION,	//�P�_����l�p�`�𐶐�
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	//UV�̃I�t�Z�b�g�P��(�ꖇ�̃p�[�e�B�N����uv�ő�l)
	float2 uvOffsetUint = { 1.0f / (float)imgSplitX,1.0f / (float)imgSplitY };
    if (imgSplitX == 0)
        uvOffsetUint.x = 0.0f;
    if (imgSplitY == 0)
        uvOffsetUint.y = 0.0f;
	// = 0.3333f,1.0f

	for (uint i = 0; i < vnum; i++)
	{
		//�X�P�[����K�p
		float4 offset = offsetArray[i];
		offset.xy *= input[0].scale;
		//���S����̃I�t�Z�b�g���r���{�[�h��]
		offset = mul(billboardMat, offset);
		//�I�t�Z�b�g�����炷
		element.svpos = input[0].pos + offset;
		//�r���[�v���W�F�N�V�����ϊ�
		element.svpos = mul(viewProjMat, element.svpos);

		element.uv = uvArray[i] * uvOffsetUint;
		element.uv += input[0].index * uvOffsetUint;

		element.color = input[0].color;
		output.Append(element);
	}
}