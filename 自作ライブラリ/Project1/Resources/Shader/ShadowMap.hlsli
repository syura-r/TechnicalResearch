cbuffer cbuff0 : register(b0)
{
    matrix viewproj; // �r���[�v���W�F�N�V�����s��
    matrix world; // ���[���h�s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
};

struct VSGrassOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD; // uv�l
};


struct VSInput
{
    float4 svpos : POSITION; // �V�X�e���p���_���W
    float3 normal : NORMAL; //�@��
    float2 uv : TEXCOORD; // uv�l
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : BONEWEIGHTS;
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
};

struct HS_CONTROL_POINT_OUTPUT
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
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
};

