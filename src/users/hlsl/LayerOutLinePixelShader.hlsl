#include"LayerShaderHeader.hlsli"

PSOutput main(GSOutput input)
{
    PSOutput output;

    //output.color = float4(ANOTHER_COLOR, 1.0f);
    //if (color)
    //{
    //    output.color.xyz = DEFAULT_COLOR;
    //}
    output.color = float4(OUTLINE_COLOR, ALPHA);
    output.depth = input.depthInView;
    output.clip = 1;
    
    return output;
}