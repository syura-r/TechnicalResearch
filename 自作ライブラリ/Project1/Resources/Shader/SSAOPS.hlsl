#include"SpriteShaderHeader.hlsli"
//SSAO�����̂��߂����̃V�F�[�_
Texture2D<float4> normtex : register(t0); //1�p�X�ڂ̖@���`��
Texture2D<float> depthtex : register(t1); //1�p�X�ڂ̐[�x�e�N�X�`��

SamplerState smp : register(s0);


static const int trycnt = 12;

//�����W�����ɕK�v
cbuffer sceneBuffer : register(b1)
{
    matrix view; //�r���[
    matrix proj; //�v���W�F�N�V����
    matrix invproj; //�t�v���W�F�N�V����
    float4 randNormal[trycnt];
};

//���݂�UV�l�����ɗ�����Ԃ�
float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}
//SSAO(��Z�p�̖��x�̂ݏ���Ԃ���΂悢)
float PSmain(VSOutput input) : SV_Target
{
    float dp = depthtex.Sample(smp, input.uv); //���݂�UV�̐[�x

	//���̍��W�𕜌�����
    float4 respos = mul(invproj, float4(input.uv * float2(2, -2) + float2(-1, 1), dp, 1));
    respos.xyz = respos.xyz / respos.w;
    //float div = 0.0f;
    float ao = 0.0f;
    float3 norm = normalize((normtex.Sample(smp, input.uv).xyz * 2) - 1);
    const float radius = 0.05f;


    //float4 vBilinearWeights[4] =
    //{
    //            //0   //1  //2    //3
    //    float4( 9/16, 3/16, 3/16, 1/16), //0
	   // float4( 3/16, 9/16, 1/16, 3/16), //1
	   // float4( 3/16, 1/16, 9/16, 3/16), //2
	   // float4( 1/16, 3/16, 3/16, 9/16), //3
    	
    //};
	if(dp<1.0f)
    {
        for (int i = 0; i < trycnt; ++i)
        {

            float3 omega = randNormal[i].xyz;

            float dt = dot(norm, omega);
            float sgn = sign(dt);
            omega *= sgn;
            float4 rpos = mul(proj, mul(view, float4(respos.xyz + omega * radius, 1)));
            rpos.xyz /= rpos.w;
            ao += step(depthtex.Sample(smp, (rpos.xy + float2(1, -1)) * float2(0.5, -0.5)), rpos.z) * dt * sgn;

        }
        ao /= (float) trycnt;
    }
    //float resultAo = lerp(ao,0, step( 1,dp));
    return 1.0f - ao;
}