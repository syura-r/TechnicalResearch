#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); //í èÌÉJÉâÅ[
Texture2D<float> texSSAO : register(t1); //SSAO

SamplerState smp : register(s0);

float Gaussian(float sigma, float x)
{
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

float JoinedBilateralGaussianBlur(float2 uv)
{
    float g_sigmaX = 3;
    float g_sigmaV = 0.03 * -0.3 + 0.001;
    float g_sigmaY = 3;
    float w, h, miplevels;
    texSSAO.GetDimensions(0, w, h, miplevels);

    float2 g_pixelSize = (1.0f / w, 1.0f / h);

    const float c_halfSamplesX = 4.;
    const float c_halfSamplesY = 4.;

    float total = 0.0;
    float ret = 0.0;

    float2 pivot = texSSAO.Sample(smp, uv);
    
    for (float iy = -c_halfSamplesY; iy <= c_halfSamplesY; iy++)
    {
        float fy = Gaussian(g_sigmaY, iy);
        float offsety = iy * g_pixelSize.y;

        for (float ix = -c_halfSamplesX; ix <= c_halfSamplesX; ix++)
        {
            float fx = Gaussian(g_sigmaX, ix);
            float offsetx = ix * g_pixelSize.x;
            
            float2 value = texSSAO.Sample(smp, uv + float2(offsetx, offsety));
                        
            float fv = Gaussian(g_sigmaV, abs(value.y - pivot.y));
            
            total += fx * fy * fv;
            ret += fx * fy * fv * value.r;
        }
    }
        
    return ret / total;
}
  

float4 PSmain(VSOutput input) : SV_TARGET
{
    //float w, h, miplevels;
    //texSSAO.GetDimensions(0, w, h, miplevels);

    //float ssao = texSSAO.Sample(smp, input.uv);
    //float4 returnColor = float4(ssao, ssao, ssao, 1.0f);
    //return returnColor;

    float4 col = tex.Sample(smp, input.uv);
    float filtered = JoinedBilateralGaussianBlur(input.uv);
    const float4 ssaoColor = float4(float3(filtered, filtered, filtered), 1.0);
    float4 returnColor = float4(col.rgb * ssaoColor.rgb, col.a);
    return returnColor;
}


