#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); //’ÊíƒJƒ‰[
Texture2D<float> texSSAO : register(t1); //SSAO

SamplerState smp : register(s0);


float4 PSmain(VSOutput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv);
    float4 returnColor = float4(col.rgb * texSSAO.Sample(smp, input.uv), col.a);
    return returnColor;
}