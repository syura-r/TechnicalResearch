#include "AreaEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp0 : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 PSmain(VSOutput input) : SV_TARGET
{
    float4 texcolor1 = tex0.Sample(smp0, input.uv + float2(0, offsetTime));
    //float4 texcolor2 = tex0.Sample(smp0, input.uv + float2(0.2f, 0.3f + offsetTime*2))*0.5f;
    ////float4 texcolor3 = tex0.Sample(smp0, input.uv + float2(0.5f, 0.61f + offsetTime));

    float4 returnColor = texcolor1 /*+ texcolor2*//* + texcolor3*/;

    //returnColor = clamp(returnColor, 0, 1);
	
	returnColor *= input.uv.y * 0.45f;
    return returnColor * color;
}
