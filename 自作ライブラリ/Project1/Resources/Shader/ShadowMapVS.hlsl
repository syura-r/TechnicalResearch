#include"ShadowMap.hlsli"

VSOutput VSmain(float4 pos : POSITION)
{
	//�s�N�Z���V�F�[�_�\�ɓn���l	
    VSOutput output;
	//output.svpos = mul(mat, pos);
    output.svpos = mul(mul(viewproj, world), pos);

    return output;
}


