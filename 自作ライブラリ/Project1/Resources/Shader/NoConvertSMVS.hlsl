#include"ShadowMap.hlsli"

VSGrassOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD )
{
	//�s�N�Z���V�F�[�_�\�ɓn���l	
    VSGrassOutput output;
    output.svpos = pos;
    output.uv = uv;
    return output;
}