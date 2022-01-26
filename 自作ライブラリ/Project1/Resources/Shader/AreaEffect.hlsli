
cbuffer cbuff0 : register(b0)
{
    matrix viewproj; // �r���[�v���W�F�N�V�����s��
    matrix world; // ���[���h�s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
    float4 color;
    matrix lightViewProj;
};
cbuffer cbuff2 : register(b2)
{
    float offsetTime;//UV�X�N���[���p
};


struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD; // uv�l
};
