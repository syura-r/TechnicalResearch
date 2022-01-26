
#include"FBXPolygonBreak.hlsli"

//VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{

//	//�@���Ƀ��[���h�s��Ɏl�X�P�[�����O�E��]��K�p
//    //float4 wnormal = normalize(mul(world, float4(normal, 0)));
//    float4 wpos = mul(world, pos);

//	//�s�N�Z���V�F�[�_�\�ɓn���l	
//    VSOutput output;
//	//output.svpos = mul(mat, pos);
    
//    output.svpos = pos;
//    output.worldpos = wpos;
//    output.normal = normal;
//    output.uv = uv;

//    return output;
//}
struct SkinOutPut
{
    float4 pos;
    float3 normal;
};
//�X�L�j���O�v�Z
SkinOutPut ComputeSkin(VSInput input)
{
//�[���N���A
    SkinOutPut output = (SkinOutPut) 0;

    uint iBone; //�v�Z����{�[���ԍ�
    float weight; //�{�[���E�F�C�g(�d��)
    matrix m; //�X�L�j���O�E�F�C�g

    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);

    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);

    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);

    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.svpos);
    output.normal += weight * mul((float3x3) m, input.normal);


    return output;
}

VSOutput VSmain(VSInput input)
{
    SkinOutPut skinned = ComputeSkin(input);
	
	//�@���Ƀ��[���h�s��Ɏl�X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));

	//�s�N�Z���V�F�[�_�\�ɓn���l	
    VSOutput output;
	//output.svpos = mul(mat, pos);
    output.svpos =  skinned.pos;
		//�@���Ƀ��[���h�s��Ɏl�X�P�[�����O�E��]��K�p
    //float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, skinned.pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = input.uv;

    return output;
}