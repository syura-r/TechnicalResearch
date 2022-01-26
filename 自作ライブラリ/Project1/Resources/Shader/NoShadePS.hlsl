#include"NoShade.hlsli"

Texture2D<float4> tex0 : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 PSmain(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex0.Sample(smp, input.uv);
	//�V�F�[�f�B���O�ɂ��F
	float4 shadecolor;
	//����x
	const float shininess = 4.0f;
	//���ʔ��ˌ�
	float3 ambient = m_ambient;
	//�g�U���ˌ�
	float3 diffuse =  m_diffuse;
	//�S�ĉ��Z����
	shadecolor.rgb = (ambient + diffuse);
	shadecolor.a = m_alpha;

	//�V�F�[�f�B���O�ɂ��F�ŕ`��
	return shadecolor * texcolor * color;
}