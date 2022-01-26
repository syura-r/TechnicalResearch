#include "Grass.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	//�@���Ƀ��[���h�s��Ɏl�X�P�[�����O�E��]��K�p
    //float4 wnormal = normalize(mul(world, float4(normal, 0)));
    //float4 wpos = mul(world, pos);

	//�s�N�Z���V�F�[�_�\�ɓn���l	
    VSOutput output;
	//output.svpos = mul(mat, pos);
    
    output.svpos = pos;
    output.worldpos = pos;
    output.normal = normal;
    output.uv = uv;

    return output;
}