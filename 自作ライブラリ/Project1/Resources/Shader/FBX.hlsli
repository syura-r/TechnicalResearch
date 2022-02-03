#include "LightStruct.hlsli"
cbuffer cbuff0 : register(b0)
{
	//matrix mat; // �R�c�ϊ��s��
	matrix viewproj;//�r���[�v���W�F�N�V�����s��
	matrix world;//���[���h�s��
	float3 cameraPos;//�J�������W(���[���h���W)
	float4 color;
};

cbuffer cbuff1 : register(b1)
{
    matrix lightCameraVP; //�r���[�v���W�F�N�V�����s��
    float3 lightCameraPos; //�J�������W(���[���h���W)
}
cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
    SpotLight spotLights[SPOTLIGHT_NUM];
    CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

static const int MAX_BONES = 32;
cbuffer skinning:register(b3)
{
    matrix matSkinning[MAX_BONES];
}
// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 shadowPos : TEXCOORD1; //�e���W
    float4 worldpos : POS; // ���[���h���W
	float3 normal:NORMAL;//�@��
	float2 uv  :TEXCOORD; // uv�l
};
struct VSInput
{
    float4 svpos : POSITION; // �V�X�e���p���_���W	
    float3 normal : NORMAL; //�@��
    float2 uv : TEXCOORD; // uv�l
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : BONEWEIGHTS;
};
struct PSOutPut
{
    float4 color : SV_TARGET0; //�ʏ�̃����_�����O����
    float4 normal : SV_TARGET1; //�@��
    float4 depth : SV_TARGET2; //�[�x�l	
};

