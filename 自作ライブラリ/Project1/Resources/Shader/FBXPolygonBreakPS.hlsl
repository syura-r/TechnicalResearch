#include"FBXPolygonBreak.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float> tex1 : register(t1); // 1番スロットに設定されたテクスチャ

SamplerState smp0 : register(s0); // 0番スロットに設定されたサンプラー
SamplerComparisonState smp1 : register(s1); // 1番スロットに設定されたサンプラー

PSOutPut PSmain(GSOutput input) 
{
	
	// テクスチャマッピング
    float4 colorSRGB = tex0.Sample(smp0, input.uv);
    float4 texcolor = pow(colorSRGB, 2.2);


    const float zlnLVP = input.shadowPos.z / input.shadowPos.w;
	
    float2 shadowMapUV = input.shadowPos.xy / input.shadowPos.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
	
    float shadow = tex1.SampleCmpLevelZero(smp1, shadowMapUV, zlnLVP - 0.0002f);
    //float shadowTex = tex1.Sample(smp1, shadowMapUV);
    float3 shadowColor = texcolor.xyz * 0.3f;
	
    texcolor.xyz = lerp(shadowColor, texcolor.xyz, shadow);

	// 光沢度
    const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

    float3 ambient = 0.76f;
    float3 m_diffuse = float3(0.5f, 0.5f, 0.5f);
    float3 m_specular = float3(0.1f, 0.1f, 0.1f);
    float m_alpha = 1;
	// シェーディングによる色
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	//平行光源
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
		// ライトに向かうベクトルと法線の内積
        float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
		// 反射光ベクトル
        float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		// 拡散反射光
        float3 diffuse = dotlightnormal * 0.80;
		// 鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * 0.10;

		// 全て加算する
        shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
    }
	//点光源
    for (int i = 0; i < POINTLIGHT_NUM; i++)
    {
			//ライトのベクトル
        float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
        float d = length(lightv);
			//正規化し、単位ベクトルにする
        lightv = normalize(lightv);
			//距離減衰係数
        float atten = lerp(0, lerp(0,1.0f/ (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
				pointLights[i].lightatten.z * d * d), step(0.5, pointLights[i].active)),
        step(0, pointLights[i].lightatten.x + pointLights[i].lightatten.y + pointLights[i].lightatten.z));
			//ライトに向かうベクトルの内積
        float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//全て加算する
        shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;

    }
	//スポットライト
    for (int i = 0; i < SPOTLIGHT_NUM; i++)
    {
			//ライトのベクトル
        float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
        float d = length(lightv);
			//正規化し、単位ベクトルにする
        lightv = normalize(lightv);
			//距離減衰係数
        float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d +
				spotLights[i].lightatten.z * d * d));
			//角度減衰
        float cos = dot(lightv, spotLights[i].lightv);
			//減衰開始角度から、減衰終了角度にかけて減衰
			//減衰開始角度の内側は1倍減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			//角度減衰を乗算
        atten *= angleatten;
			//ライトに向かうベクトルの内積
        float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
        float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//全て加算する
        shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor * clamp(spotLights[i].active, 0, 1.0f);
    }
	//丸影
    for (int i = 0; i < CIRCLESHADOW_NUM; i++)
    {
			//オブジェクト表面からキャスターへのベクトル
        float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			//投影方向での距離
        float d = dot(casterv, circleShadows[i].dir);
			//距離減衰係数
        float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d +
				circleShadows[i].atten.z * d * d));
			//距離がマイナスなら0になる
        atten *= step(0, d);
			//仮想ライトの座標
        float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//オブジェクト表面からライトへのベクトル(単位ベクトル)
        float3 lightv = normalize(lightpos - input.worldpos.xyz);
			//角度減衰
        float cos = dot(lightv, circleShadows[i].dir);
			//減衰開始角度から、減衰終了角度にかけて減衰
			//減衰開始角度の内側は1倍減衰終了角度の外側は0倍の輝度
        float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			//角度減衰を乗算
        atten *= angleatten;
			//全て減する
        shadecolor.rgb -= atten * clamp(circleShadows[i].active, 0, 1.0f);
    }
	// シェーディングによる色で描画
   // return shadecolor *  texcolor;

    PSOutPut outPut;
	
    const float TurningPoint = 0.15f;
    float4 returnColor = (shadecolor * texcolor * color) * smoothstep(1.0f - TurningPoint, 1.0f, 1 - input.destruction)
			+ float4(1, 1, 1, 1) * smoothstep(0, TurningPoint, input.destruction)
			- lerp(0, float4(0.8f, 0.8f, 0.8f, 0) * smoothstep(TurningPoint, TurningPoint * 10, input.destruction), step(0.5, _OnEasing));
    outPut.color = pow(returnColor, 1.0 / 2.2);
    outPut.normal.rgb = float3((input.normal.xyz + 1.0f) / 2.0f);
    outPut.normal.a = 1;
    outPut.depth = float4(input.svpos.z, input.svpos.z, input.svpos.z, 1.0f);
	
    return outPut;

}