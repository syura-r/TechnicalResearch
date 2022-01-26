#include"ShadowMap.hlsli"

static const int MAX_BONES = 32;
cbuffer skinning : register(b1)
{
    matrix matSkinning[MAX_BONES];
}


//�X�L�j���O�v�Z
float4 ComputeSkin(VSInput input)
{
//�[���N���A
    float4 output = float4(0,0,0,0);

    uint iBone; //�v�Z����{�[���ԍ�
    float weight; //�{�[���E�F�C�g(�d��)
    matrix m; //�X�L�j���O�E�F�C�g

    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output += weight * mul(m, input.svpos);

    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output += weight * mul(m, input.svpos);

    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output += weight * mul(m, input.svpos);

    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output += weight * mul(m, input.svpos);


    return output;
}

VSOutput VSmain(VSInput input)
{
    float4 skinnedPos = ComputeSkin(input);
	
	//�s�N�Z���V�F�[�_�\�ɓn���l	
    VSOutput output;
	//output.svpos = mul(mat, pos);
    output.svpos = mul(mul(viewproj, world), skinnedPos);

    return output;
}