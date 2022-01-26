#include"PolygonBreak.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float> tex1 : register(t1); // 1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp0 : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerComparisonState smp1 : register(s1); // 1�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
//SamplerState smp1 : register(s1); // 1�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutPut PSmain(GSOutput input) : SV_TARGET
{
    	//�e�N�X�`���}�b�s���O
    float4 colorSRGB = tex0.Sample(smp0, input.uv);
    float4 texcolor = pow(colorSRGB, 2.2);


    const float zlnLVP = input.shadowPos.z / input.shadowPos.w;
	
    float2 shadowMapUV = input.shadowPos.xy / input.shadowPos.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
	
    float shadow = tex1.SampleCmpLevelZero(smp1, shadowMapUV, zlnLVP - 0.0002f);
    float3 shadowColor = texcolor.xyz * 0.3f;
	
    texcolor.xyz = lerp(shadowColor, texcolor.xyz, shadow);

	//�V�F�[�f�B���O�ɂ��F
		// ����x
    const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
    float3 ambient = m_ambient*0.5f;

	// �V�F�[�f�B���O�ɂ��F
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	//���s����
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
		// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
		// ���ˌ��x�N�g��
        float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		// �g�U���ˌ�
        float3 diffuse = dotlightnormal * 0.80;
        float3 halfVec = normalize(dirLights[i].lightv + eyedir);

		// ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, halfVec)), shininess) * 0.10;

		// �S�ĉ��Z����
        shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
    }
	//�_����
    for (int i = 0; i < POINTLIGHT_NUM; i++)
    {
			//���C�g�̃x�N�g��
        float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//�x�N�g���̒���
        float d = length(lightv);
			//���K�����A�P�ʃx�N�g���ɂ���
        lightv = normalize(lightv);
			//���������W��
        float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
				pointLights[i].lightatten.z * d * d);
			//���C�g�Ɍ������x�N�g���̓���
        float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
        float3 halfVec = normalize(lightv + eyedir);
			//���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, halfVec)), shininess) * m_specular;
			//�S�ĉ��Z����
        shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor * pointLights[i].active;

    }
	//�X�|�b�g���C�g
    for (int i = 0; i < SPOTLIGHT_NUM; i++)
    {
			//���C�g�̃x�N�g��
        float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			//�x�N�g���̒���
        float d = length(lightv);
			//���K�����A�P�ʃx�N�g���ɂ���
        lightv = normalize(lightv);
			//���������W��
        float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d +
				spotLights[i].lightatten.z * d * d));
			//�p�x����
        float cos = dot(lightv, spotLights[i].lightv);
			//�����J�n�p�x����A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓�����1�{�����I���p�x�̊O����0�{�̋P�x
        float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			//�p�x��������Z
        atten *= angleatten;
			//���C�g�Ɍ������x�N�g���̓���
        float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
        float3 halfVec = normalize(lightv + eyedir);
        float3 specular = pow(saturate(dot(reflect, halfVec)), shininess) * m_specular;
			//�S�ĉ��Z����
        shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor * clamp(spotLights[i].active, 0, 1.0f);
    }
	//�ۉe
    for (int i = 0; i < CIRCLESHADOW_NUM; i++)
    {
			//�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
        float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			//���e�����ł̋���
        float d = dot(casterv, circleShadows[i].dir);
			//���������W��
        float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d +
				circleShadows[i].atten.z * d * d));
			//�������}�C�i�X�Ȃ�0�ɂȂ�
        atten *= step(0, d);
			//���z���C�g�̍��W
        float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g��(�P�ʃx�N�g��)
        float3 lightv = normalize(lightpos - input.worldpos.xyz);
			//�p�x����
        float cos = dot(lightv, circleShadows[i].dir);
			//�����J�n�p�x����A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓�����1�{�����I���p�x�̊O����0�{�̋P�x
        float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			//�p�x��������Z
        atten *= angleatten;
			//�S�Č�����
        shadecolor.rgb -= atten * clamp(circleShadows[i].active, 0, 1.0f);
    }
   // float dColor = 1 - _Destruction;
	// �V�F�[�f�B���O�ɂ��F�ŕ`��
   // return shadecolor *  texcolor;
    PSOutPut outPut;
    const float TurningPoint = 0.05f;
    outPut.color = pow((shadecolor * texcolor * color), 1.0 / 2.2);
    outPut.normal.rgb = float3((input.normal.xyz + 1.0f) / 2.0f);
    outPut.normal.a = 1;
    return outPut;
   // return (shadecolor * texcolor * color) * lerp(smoothstep(1.0f - TurningPoint, 1.0f, 1 - input.destruction), 1, step(1, _On4Vertex))
   // +float4(1, 1, 1, 1) * smoothstep(0, TurningPoint, input.destruction) * lerp(1, 1 - step(TurningPoint, input.destruction), step(1, _On4Vertex))
			//- float4(0.8f, 0.8f, 0.8f, 0) * smoothstep(TurningPoint, TurningPoint * 10, input.destruction) * lerp(1, 0, step(1, _On4Vertex));
   // return (shadecolor * texcolor * color) * lerp(smoothstep(1.0f - TurningPoint, 1.0f, 1 - input.destruction), 1, step(1, _On4Vertex))
			//+ float4(1, 1, 1, 1) * smoothstep(0, TurningPoint, input.destruction) * lerp(1, 0, step(1, _On4Vertex))
			//- float4(0.8f, 0.8f, 0.8f, 0) * smoothstep(TurningPoint, TurningPoint * 10, input.destruction) * lerp(1, 0, step(1, _On4Vertex));

}