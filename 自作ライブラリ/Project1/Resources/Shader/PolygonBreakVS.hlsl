#include"PolygonBreak.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wpos = mul(world, pos);

	//�s�N�Z���V�F�[�_�\�ɓn���l	
    VSOutput output;
    
    output.svpos = pos;
    output.worldpos = wpos;
    output.normal = normal;
    output.uv = uv;
    return output;
}