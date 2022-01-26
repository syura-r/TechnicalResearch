#include"NoShade.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	//�@���Ƀ��[���h�s��Ɏl�X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	//�s�N�Z���V�F�[�_�\�ɓn���l	
	VSOutput output;
	//output.svpos = mul(mat, pos);
	output.svpos = mul(mul(viewproj, world), pos);
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;

	return output;
}