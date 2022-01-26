#include"BasicShaderHeader.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
struct PSOutPut
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
    float4 target2 : SV_TARGET2;
	
};
PSOutPut PSmain(VSOutput input)
{
    PSOutPut output;

	// �e�N�X�`���}�b�s���O
    float4 texcolor = tex0.Sample(smp, input.uv);

	// ����x
    const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
    float3 ambient = 0.26;

	// �V�F�[�f�B���O�ɂ��F
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
    float4 a = float4(0, 0, 0, 1);
	//���s����
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
		// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
        float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
		// ���ˌ��x�N�g��
        float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		// �g�U���ˌ�
        float3 diffuse = dotlightnormal * 0.80;
		// ���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * 0.10;

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
        float atten = 1.0f / lerp(1, (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
				pointLights[i].lightatten.z * d * d), step(0.5, pointLights[i].active));
			//���C�g�Ɍ������x�N�g���̓���
        float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
        float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
        float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//�S�ĉ��Z����
        shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;

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
        float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
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
	// �V�F�[�f�B���O�ɂ��F�ŕ`��
    float4 rColor = /*shadecolor **/ texcolor * color;
	
	//�ʏ�̃����_�����O����
    output.target0 = rColor /** shadecolor*/;
	
	//�F���](�u���[���Ƃł͎g�p���ĂȂ�)
    output.target1 = float4(1 - rColor.rgb, 1);

	//���m�N���ɂ��Ĉ��ȏ�̖��邳�����镔���̂ݏo��
    float y = dot(float3(0.2299f, 0.587f, 0.114f), rColor.w);
    output.target2 = lerp(0.0f, rColor, step(0.8f, rColor.z));
	
    return output;
}