#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex0:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);  //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 PSmain(VSOutput input) :SV_TARGET
{
    float4 colorSRGB = tex0.Sample(smp, input.uv);
    float4 colorLINEAR = pow(colorSRGB, 2.2);
    float4 returnColor = colorLINEAR * color;
    returnColor = pow(returnColor, 1.0 / 2.2);
	//if(returnColor.w <= 0)
 //       discard;
    return returnColor;

}