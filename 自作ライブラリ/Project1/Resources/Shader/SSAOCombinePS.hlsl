#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); //通常カラー
Texture2D<float> texSSAO : register(t1); //SSAO

SamplerState smp : register(s0);


float4 PSmain(VSOutput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv);
    float4 returnColor = float4(col.rgb * texSSAO.Sample(smp, input.uv), col.a);
    //float ssao = texSSAO.Sample(smp, input.uv);
    //float4 returnColor = float4(ssao,ssao,ssao,ssao);

    return returnColor;
}