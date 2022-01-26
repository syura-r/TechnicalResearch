#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex0 : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 GetRet(const float w,const float h,float2 uv)
{
    float dx = 1.0f / w;
    float dy = 1.0f / h;
    float4 ret = float4(0, 0, 0, 0);
	//�ŏ�i
    ret += tex0.Sample(smp, uv + float2(-2 * dx, 2 * dy)) * 1 / 256;
    ret += tex0.Sample(smp, uv + float2(-1 * dx, 2 * dy)) * 4 / 256;
    ret += tex0.Sample(smp, uv + float2(0 * dx, 2 * dy)) * 6 / 256;
    ret += tex0.Sample(smp, uv + float2(1 * dx, 2 * dy)) * 4 / 256;
    ret += tex0.Sample(smp, uv + float2(2 * dx, 2 * dy)) * 1 / 256;

	//1��i
    ret += tex0.Sample(smp, uv + float2(-2 * dx, 1 * dy)) * 4 / 256;
    ret += tex0.Sample(smp, uv + float2(-1 * dx, 1 * dy)) * 16 / 256;
    ret += tex0.Sample(smp, uv + float2(0 * dx, 1 * dy)) * 24 / 256;
    ret += tex0.Sample(smp, uv + float2(1 * dx, 1 * dy)) * 16 / 256;
    ret += tex0.Sample(smp, uv + float2(2 * dx, 1 * dy)) * 4 / 256;

	//���i
    ret += tex0.Sample(smp, uv + float2(-2 * dx, 0 * dy)) * 6 / 256;
    ret += tex0.Sample(smp, uv + float2(-1 * dx, 0 * dy)) * 24 / 256;
    ret += tex0.Sample(smp, uv + float2(0 * dx, 0 * dy)) * 36 / 256;
    ret += tex0.Sample(smp, uv + float2(1 * dx, 0 * dy)) * 24 / 256;
    ret += tex0.Sample(smp, uv + float2(2 * dx, 0 * dy)) * 6 / 256;

    //1���i
    ret += tex0.Sample(smp, uv + float2(-2 * dx, -1 * dy)) * 4 / 256;
    ret += tex0.Sample(smp, uv + float2(-1 * dx, -1 * dy)) * 16 / 256;
    ret += tex0.Sample(smp, uv + float2(0 * dx, -1 * dy)) * 24 / 256;
    ret += tex0.Sample(smp, uv + float2(1 * dx, -1 * dy)) * 16 / 256;
    ret += tex0.Sample(smp, uv + float2(2 * dx, -1 * dy)) * 4 / 256;

	//�ŏ�i
    ret += tex0.Sample(smp, uv + float2(-2 * dx, -2 * dy)) * 1 / 256;
    ret += tex0.Sample(smp, uv + float2(-1 * dx, -2 * dy)) * 4 / 256;
    ret += tex0.Sample(smp, uv + float2(0 * dx, -2 * dy)) * 6 / 256;
    ret += tex0.Sample(smp, uv + float2(1 * dx, -2 * dy)) * 4 / 256;
    ret += tex0.Sample(smp, uv + float2(2 * dx, -2 * dy)) * 1 / 256;

    return ret;
}

float4 PSmain(VSOutput input) : SV_TARGET
{
    float w, h, miplevels;
    tex0.GetDimensions(0, w, h, miplevels);

    return /*tex0.Sample(smp, input.uv) +*/ GetRet(w,h,input.uv);
}