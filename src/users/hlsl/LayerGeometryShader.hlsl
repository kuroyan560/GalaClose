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
        element.depthInView = input[i].pos.z; //ƒJƒƒ‰‚©‚çŒ©‚½Z
        element.ray = normalize(input[i].pos.xyz - cameraPos);
        
        //‰Ÿ‚µž‚Ý‚Ì‰e‹¿
        float4 resultPos = input[i].pos;
        
        float radiusOffset = -pushedRate * LAYER_OFFSET_RADIUS;
        
        float2 center = { 0, 0 };
        float2 vec = normalize(resultPos.xy - center);
        if (resultPos.x == 0.0f && resultPos.y == 0)
            vec = float2(0, 0);
        
        resultPos.x += vec.x * radiusOffset;
        resultPos.y += vec.y * radiusOffset;
        
        if (sign(input[i].pos.x) != sign(resultPos.x))    //•„†‚ªˆá‚Á‚½‚ç
        {
            resultPos.x = 0.0f; //ƒ[ƒ‚É‚µ‚Ä”½‘Î‘¤‚É“Ë‚«”²‚¯‚é‚Ì‚ð–h‚®
        }
        if (sign(input[i].pos.y) != sign(resultPos.y))    //•„†‚ªˆá‚Á‚½‚ç
        {
            resultPos.y = 0.0f; //ƒ[ƒ‚É‚µ‚Ä”½‘Î‘¤‚É“Ë‚«”²‚¯‚é‚Ì‚ð–h‚®
        }
        
        element.svpos = mul(proj, mul(view, resultPos)); //ƒrƒ…[ƒvƒƒWƒFƒNƒVƒ‡ƒ“•ÏŠ·;
	
        output.Append(element);
    }
}