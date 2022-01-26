#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex0 : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
cbuffer cbuff1 : register(b1)
{
	//���S����̕`��͈�
    float drawDistance;
};


float4 PSmain(VSOutput input) : SV_TARGET
{

    if (length(input.uv - float2(0.5f, 0.5f)) > drawDistance)
        discard;
    float4 returnColor = tex0.Sample(smp, input.uv) * color;

	//if(returnColor.w <= 0)
 //       discard;
    return returnColor;

}