#include"LayerShaderHeader.hlsli"

//[maxvertexcount(6)]
[maxvertexcount(4)]
void main(
	line VSOutput input[2],
	inout TriangleStream<GSOutput> output
)
{
    GSOutput element[2];
    
    for (int i = 0; i < 2; ++i)
    {
        element[i].depthInView = input[i].pos.z; //ƒJƒƒ‰‚©‚çŒ©‚½Z
        element[i].ray = normalize(input[i].pos.xyz - cameraPos);
        
        //‰Ÿ‚µž‚Ý‚Ì‰e‹¿
        float4 resultPos = input[i].pos;
        if (-0.01f < resultPos.x && resultPos.x < 0.01f)
        {
            resultPos.x = 0.0f;
        }
        
        if (0 < resultPos.z)
            resultPos.z += 0.1f;
        else
            resultPos.z -= 0.1f;
        
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
        
        element[i].svpos = mul(proj, mul(view, resultPos)); //ƒrƒ…[ƒvƒƒWƒFƒNƒVƒ‡ƒ“•ÏŠ·;
    }
    
    GSOutput result[4];
    
    result[0] = element[0];
    if (element[0].svpos.x != element[1].svpos.x)
    {
        result[0].svpos.y += LAYER_OUTLINE_OFFSET_THICKNESS * (0 ? -1 : 1);
    }
    else
    {
        result[0].svpos.x += LAYER_OUTLINE_OFFSET_THICKNESS / 2.0f * (0 ? 1 : -1);
    }
    output.Append(result[0]);
    
    result[1] = element[0];
    if (element[0].svpos.x != element[1].svpos.x)
    {
        result[1].svpos.y += LAYER_OUTLINE_OFFSET_THICKNESS * (1 ? -1 : 1);
    }
    else
    {
        result[1].svpos.x += LAYER_OUTLINE_OFFSET_THICKNESS / 2.0f * (1 ? 1 : -1);
    }
    output.Append(result[1]);
    
    result[2] = element[0];
    if (element[0].svpos.x != element[1].svpos.x)
    {
        result[2].svpos.y += LAYER_OUTLINE_OFFSET_THICKNESS * (0 ? -1 : 1);
    }
    else
    {
        result[2].svpos.x += LAYER_OUTLINE_OFFSET_THICKNESS / 2.0f * (0 ? 1 : -1);
    }
    output.Append(result[2]);
    
    result[3] = element[0];
    if (element[0].svpos.x != element[1].svpos.x)
    {
        result[3].svpos.y += LAYER_OUTLINE_OFFSET_THICKNESS * (1 ? -1 : 1);
    }
    else
    {
        result[3].svpos.x += LAYER_OUTLINE_OFFSET_THICKNESS / 2.0f * (1 ? 1 : -1);
    }
    output.Append(result[3]);
        //for (int i = 0; i < 2; ++i)
        //{
        //    for (int j = 0; j < 2; ++j)
        //    {
        //        GSOutput result = element[i];
            


        //    }
        //}
    //output.Append(result);
}