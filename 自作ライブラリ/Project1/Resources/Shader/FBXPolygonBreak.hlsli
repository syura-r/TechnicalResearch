#include"LightStruct.hlsli"
cbuffer cbuff0 : register(b0)
{
    matrix viewproj; // �r���[�v���W�F�N�V�����s��
    matrix world; // ���[���h�s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
    float4 color;
    uint drawShadow;
};

//cbuffer cbuff1 : register(b1)
//{
//    float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
//    float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
//    float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
//    float m_alpha : packoffset(c2.w); // �A���t�@
//}

cbuffer cbuff4 : register(b4)
{
    float _Destruction; //����x����
    float _ScaleFactor; //�X�P�[���̕ω���
    float _PositionFactor; //�|�W�V�����̕ω���
    float _RotationFactor; //��]�̕ω���
    uint _Tessellation;//�|���S�������x
    uint _OnEasing;//�C�[�W���O�ŕ������邩
}
cbuffer cbuff1 : register(b1)
{
    matrix lightCameraVP; //�r���[�v���W�F�N�V�����s��
    float3 lightCameraPos; //�J�������W(���[���h���W)
}

static const int MAX_BONES = 32;
cbuffer skinning : register(b3)
{
    matrix matSkinning[MAX_BONES];
}

struct VSInput
{
    float4 svpos : POSITION; // �V�X�e���p���_���W	
    float3 normal : NORMAL; //�@��
    float2 uv : TEXCOORD; // uv�l
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : BONEWEIGHTS;
};

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
    float4 shadowPos : TEXCOORD1; //�e���W
    float4 worldpos : POSITION; //���[���h���W
    float3 normal : NORMAL; //�@��
    float2 uv : TEXCOORD; // uv�l
    float destruction : TEXCOORD2;
};

struct PSOutPut
{
    float4 color : SV_TARGET0; //�ʏ�̃����_�����O����
    float4 normal : SV_TARGET1; //�@��
    float4 depth : SV_TARGET2; //�[�x�l	
};


struct HS_CONTROL_POINT_OUTPUT
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 worldpos : POSITION; // ���[���h���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv�l
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3


struct DS_OUTPUT
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float4 worldpos : POSITION; // ���[���h���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD; // uv�l
};

