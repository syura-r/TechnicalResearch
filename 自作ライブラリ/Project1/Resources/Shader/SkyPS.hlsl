#include"Sky.hlsli"

//float2 sunPos = float2(0.54f, 0.8f);

float3 getSky(float2 uv)
{
    float atmosphere = sqrt(1.0 - uv.y);
    float3 skyColor = float3(0.2, 0.4, 0.8);
    
    float scatter = pow(0.8f , 1.0 / 15.0);
    scatter = 1.0 - clamp(scatter, 0.8, 1.0);
    
    float3 scatterColor = lerp(float3(1,1,1), float3(1.0, 0.3, 0.0) * 1.5, scatter);
    return lerp(skyColor, float3(scatterColor), atmosphere / 1.3);
    
}

float3 getSun(float2 uv)
{
    float sun = 1.0 - distance(uv, float2(0.54f, 0.8f));
    sun = clamp(sun, 0.0, 1.0);
    
    float glow = sun;
    glow = clamp(glow, 0.0, 1.0);
    
    sun = pow(sun, 1000);
    sun *= 100.0;
    sun = clamp(sun, 0.0, 1.0);
    
    glow = pow(glow, 60) * 1.0;
    glow = pow(glow, (uv.y));
    glow = clamp(glow, 0.0, 1.0);
    
    sun *= pow(dot(uv.y, uv.y), 1.0 / 1.65);
    
    glow *= pow(dot(uv.y, uv.y), 1.0 / 2.0);
    
    sun += glow;
    
    float3 sunColor = float3(1.0, 0.6, 0.05) * sun;
    
    return float3(sunColor);
}

PSOutPut PSmain(VSOutput input)
{
    PSOutPut outPut;
    float2 uv = input.uv;	
    float3 sky = getSky(uv);
    float3 sun = getSun(uv);
    
    outPut.normal.rgb = float3((input.normal.xyz + 1.0f) / 2.0f);
    outPut.normal.a = 1;
    outPut.color =float4(sky + sun, 1.0);
    return outPut;
}