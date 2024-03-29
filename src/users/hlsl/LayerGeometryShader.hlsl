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
        element.depthInView = input[i].pos.z; //カメラから見たZ
        element.ray = normalize(input[i].pos.xyz - cameraPos);
        
        //押し込みの影響
        float4 resultPos = input[i].pos;
        
        float radiusOffset = -pushedRate * LAYER_OFFSET_RADIUS;
        
        float2 center = { 0, 0 };
        float2 vec = normalize(resultPos.xy - center);
        if (resultPos.x == 0.0f && resultPos.y == 0)
            vec = float2(0, 0);
        
        resultPos.x += vec.x * radiusOffset;
        resultPos.y += vec.y * radiusOffset;
        
        if (sign(input[i].pos.x) != sign(resultPos.x))    //符号が違ったら
        {
            resultPos.x = 0.0f; //ゼロにして反対側に突き抜けるのを防ぐ
        }
        if (sign(input[i].pos.y) != sign(resultPos.y))    //符号が違ったら
        {
            resultPos.y = 0.0f; //ゼロにして反対側に突き抜けるのを防ぐ
        }
        
        element.svpos = mul(proj, mul(view, resultPos)); //ビュープロジェクション変換;
	
        output.Append(element);
    }
}