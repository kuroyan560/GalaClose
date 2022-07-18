#include"LayerShaderHeader.hlsli"

[maxvertexcount(3)]
void main(
	triangle VSOutput input[3],
	inout TriangleStream< GSOutput > output
)
{
    for (int i = 0; i < 3;++i)
    {
        GSOutput element;
        element.depthInView = input[i].pos.z; //�J�������猩��Z
        element.ray = normalize(input[i].pos.xyz - cameraPos);
        
        //�������݂̉e��
        float4 resultPos = input[i].pos;
        
        float radiusOffset = -pushedRate * LAYER_OFFSET_RADIUS;
        
        float2 center = { 0, 0 };
        float2 vec = normalize(resultPos.xy - center);
        if (resultPos.x == 0.0f && resultPos.y == 0)
            vec = float2(0, 0);
        
        resultPos.x += vec.x * radiusOffset;
        resultPos.y += vec.y * radiusOffset;
        
        if (sign(input[i].pos.x) != sign(resultPos.x))    //�������������
        {
            resultPos.x = 0.0f; //�[���ɂ��Ĕ��Α��ɓ˂�������̂�h��
        }
        if (sign(input[i].pos.y) != sign(resultPos.y))    //�������������
        {
            resultPos.y = 0.0f; //�[���ɂ��Ĕ��Α��ɓ˂�������̂�h��
        }
        
        element.svpos = mul(proj, mul(view, resultPos)); //�r���[�v���W�F�N�V�����ϊ�;
	
        output.Append(element);
    }
}