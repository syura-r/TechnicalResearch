#include"LightStruct.hlsli"
cbuffer cbuff0 : register(b0)
{
    matrix viewproj; // �r���[�v���W�F�N�V�����s��
    matrix world; // ���[���h�s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
    float4 color;
};

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); // �A���t�@
}

cbuffer cbuff3 : register(b3)
{
    float3 playerPos;
    float4 GrassBottomColor;
    float4 GrassTopColor;
    float2 AirDir;
    float AirPower;
    uint Time;
	float playerRadius;
    float GrassWidth;
    float GrassHeight;
	
}


cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
    SpotLight spotLights[SPOTLIGHT_NUM];
    CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 worldpos : POSITION; // ���[���h���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv�l
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 worldpos : POSITION; // ���[���h���W
    float4 color : COLOR;
    float3 normal : NORMAL; //�@��
};

