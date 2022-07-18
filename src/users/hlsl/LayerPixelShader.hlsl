#include"LayerShaderHeader.hlsli"

PSOutput main(GSOutput input)
{
    PSOutput output;

    output.color = float4(DEFAULT_COLOR, ALPHA);
	if(color)
    {
        output.color.xyz = ANOTHER_COLOR;
    }
    output.depth = input.depthInView;
    output.clip = 1;
    
	return output;
}