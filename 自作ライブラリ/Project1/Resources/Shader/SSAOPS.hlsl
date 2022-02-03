#include"SpriteShaderHeader.hlsli"
//SSAO処理のためだけのシェーダ
Texture2D<float4> normtex : register(t0); //1パス目の法線描画
Texture2D<float> depthtex : register(t1); //1パス目の深度テクスチャ

SamplerState smp : register(s0);


static const int trycnt = 12;

//元座標復元に必要
cbuffer sceneBuffer : register(b1)
{
    matrix view; //ビュー
    matrix proj; //プロジェクション
    matrix invproj; //逆プロジェクション
    float4 randNormal[trycnt];
};

//現在のUV値を元に乱数を返す
float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}
//SSAO(乗算用の明度のみ情報を返せればよい)
float PSmain(VSOutput input) : SV_Target
{
    float dp = depthtex.Sample(smp, input.uv); //現在のUVの深度

	//元の座標を復元する
    float4 respos = mul(invproj, float4(input.uv * float2(2, -2) + float2(-1, 1), dp, 1));
    respos.xyz = respos.xyz / respos.w;
    //float div = 0.0f;
    float ao = 0.0f;
    float3 norm = normalize((normtex.Sample(smp, input.uv).xyz * 2) - 1);
    const float radius = 0.05f;

	if(dp<1.0f)
    {
        for (int i = 0; i < trycnt; ++i)
        {

            float3 omega = randNormal[i].xyz;

            float dt = dot(norm, omega);
            float sgn = sign(dt);
            omega *= sgn;
            float4 rpos = mul(proj, mul(view, float4(respos.xyz + omega * radius, 1)));
            rpos.xyz /= rpos.w;
            ao += step(depthtex.Sample(smp, (rpos.xy + float2(1, -1)) * float2(0.5, -0.5)), rpos.z) * dt * sgn;

        }
        ao /= (float) trycnt;
    }
    //float resultAo = lerp(ao,0, step( 1,dp));
    return 1.0f - ao;
}


//const float2 c_offset[4] =
//{
//    float2(0., 0),
//    float2(0., 1),
//    float2(1., 0),
//    float2(1., 1)
//};

//float4 JoinedBilateralUpsample(float2 P)
//{ // based on: https://johanneskopf.de/publications/jbu/paper/FinalPaper_0185.pdf
//  //           https://bartwronski.com/2019/09/22/local-linear-models-guided-filter/
//  //		   https://www.shadertoy.com/view/MllSzX
    
//    float2 halfP = 0.5 * P;
//    float2 c_textureSize = iChannelResolution[2].xy;
//    float2 c_texelSize = 1.0 / c_textureSize;
//    float2 pixel = halfP * c_textureSize + 0.5;
//    float2 f = fract(pixel);
//    pixel = (floor(pixel) / c_textureSize) - float2(c_texelSize / 2.0);
    
//    float4 I = textureLod(iChannel1, P, 0.0);
        
//    float4 Z00 = textureLod(iChannel2, pixel + c_texelSize * c_offset[0], 0.0);
//    float4 Z01 = textureLod(iChannel2, pixel + c_texelSize * c_offset[1], 0.0);
//    float4 Z10 = textureLod(iChannel2, pixel + c_texelSize * c_offset[2], 0.0);
//    float4 Z11 = textureLod(iChannel2, pixel + c_texelSize * c_offset[3], 0.0);
    
//    float tex00 = textureLod(iChannel0, pixel + c_texelSize * c_offset[0], 0.0).r;
//    float tex01 = textureLod(iChannel0, pixel + c_texelSize * c_offset[1], 0.0).r;
//    float tex10 = textureLod(iChannel0, pixel + c_texelSize * c_offset[2], 0.0).r;
//    float tex11 = textureLod(iChannel0, pixel + c_texelSize * c_offset[3], 0.0).r;
       
//    float sigmaV = 0.002;
//    //    wXX = bilateral gaussian weight from depth * bilinear weight
//    float w00 = Gaussian(sigmaV, abs(I.w - Z00.w)) * (1. - f.x) * (1. - f.y);
//    float w01 = Gaussian(sigmaV, abs(I.w - Z01.w)) * (1. - f.x) * f.y;
//    float w10 = Gaussian(sigmaV, abs(I.w - Z10.w)) * f.x * (1. - f.y);
//    float w11 = Gaussian(sigmaV, abs(I.w - Z11.w)) * f.x * f.y;
        
//    return float4((w00 * tex00 + w01 * tex01 + w10 * tex10 + w11 * tex11) / (w00 + w01 + w10 + w11));
//}

//void mainImage(out float4 fragColor, in float2 fragCoord)
//{
//    float2 uv = F.xy / R.xy;
//    float gamma = 0.4545;
    
//    fragColor = JoinedBilateralUpsample(uv);
    
//    fragColor *= smoothstep(0.0, 0.001, abs(uv.x - 0.25));
//    fragColor *= smoothstep(0.0, 0.001, abs(uv.x - 0.50));
//    fragColor *= smoothstep(0.0, 0.001, abs(uv.x - 0.75));
    
//    gamma = 4.0; // exaggerate AO results   
       
//    fragColor = float4(pow(fragColor.rgb, float3(gamma,gamma,gamma)), 1.0);
//}