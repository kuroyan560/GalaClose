#include"PrimitiveParticleHeader.hlsli"

static const float PI = 3.14159265359f;

static const uint vnum[1] = { 36 };


//CUBE ====================================
static const int3 cubeOffset[8] =
{
    { -1, -1, -1 },
    { 1, -1, -1 },
    { -1, 1, -1 },
    { 1, 1, -1 },
    { -1, -1, 1 },
    { 1, -1, 1 },
    { -1, 1, 1 },
    { 1, 1, 1 }
};
static const int cubeOffsetIdx[vnum[0]] =
{
    0, 2, 1,    //��O�P
    1, 2, 3,    //��O2
    1, 3, 5,    //�E1
    5, 3, 7,    //�E�Q
    5, 7, 4,    //��1
    4, 7, 6,    //��2
    4, 6, 0,    //��1
    0, 6, 2,    //��2
    2, 6, 3,    //��1
    3, 6, 7,    //��2
    0, 1, 5,    //��1
    0, 5, 4 ,   //��2
};
static const float3 cubeNormal[6] =
{
    { 0, 0, -1 },
    { 1, 0, 0 },
    { 0, 0, 1 },
    { -1, 0, 0 },
    { 0, 1, 0 },
    { 0, -1, 0 }
};

//==============================

//��]�s������߂�
matrix GetRoateMat(float3 Angle)
{
    //���W�A���ɒ���
    Angle.x = PI / 180 * Angle.x;
    Angle.y = PI / 180 * Angle.y;
    Angle.z = PI / 180 * Angle.z;
    
    matrix matX =
    {
        1, 0, 0, 0,
        0, cos(Angle.x), -sin(Angle.x), 0,
        0, sin(Angle.x), cos(Angle.x), 0,
        0, 0, 0, 1
    };
    matrix matY =
    {
        cos(Angle.y), -sin(Angle.y), 0, 0,
        sin(Angle.y), cos(Angle.y), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    matrix matZ =
    {
        cos(Angle.z), 0, sin(Angle.z), 0,
        0, 1, 0, 0,
        -sin(Angle.z), 0, cos(Angle.z), 0,
        0, 0, 0, 1
    };

    return mul(mul(matX, matY), matZ);
}

[maxvertexcount(64)]
void main(
	point VSOutput input[1] : POSITION,		//�P�_����v���~�e�B�u����
	inout TriangleStream< GSOutput > output
)
{
    GSOutput element;
    //�J���[�͂��̂܂ܓn��
    element.color = input[0].color;
    
    float4 center = input[0].pos;   //���S
    matrix rotateMat = GetRoateMat(input[0].rotate); //��]�p�x
    float radius = input[0].radius;
    
    //�`��ɂ���Ē��_�����ς��
    for (uint i = 0; i < vnum[input[0].primitive]; ++i)
    {
        //CUBE
        if(input[0].primitive == 0)
        {
            float side = radius * (2 / sqrt(3)); //�ӂ̒���
            int index = cubeOffsetIdx[i];
            float4 pos = center;
            pos.x += cubeOffset[index].x * side;
            pos.y += cubeOffset[index].y * side;
            pos.z += cubeOffset[index].z * side;
            
            pos = center + mul(rotateMat, pos - center);
            
            index = i / 6;
            element.normal = cubeNormal[index];
            element.ray = normalize(pos.xyz - cameraPos);
            
            pos = mul(proj,mul(view, pos)); //�r���[�ϊ�
            element.svpos = pos;
        }
        
        output.Append(element);
    }
}