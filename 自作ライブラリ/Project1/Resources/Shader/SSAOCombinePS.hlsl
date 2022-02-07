#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); //�ʏ�J���[
Texture2D<float> texSSAO : register(t1); //SSAO
//-----------�o�C���e�����A�b�v�T���v�����O�p---------------
Texture2D<float4> normTex : register(t2);     //�ʏ�T�C�Y�̖@���e�N�X�`��
Texture2D<float> depthTex : register(t3);     //�ʏ�T�C�Y�̐[�x�e�N�X�`��
Texture2D<float4> halfNormTex : register(t4); //1/4�T�C�Y�̖@���e�N�X�`��
Texture2D<float> halfDepthTex : register(t5); //1/4�T�C�Y�̐[�x�e�N�X�`��
//---------------------------------------------------------
SamplerState smp : register(s0);

float Gaussian(float sigma, float x)
{
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

float JoinedBilateralGaussianBlur(float2 uv)
{
    float g_sigmaX = 3;
    float g_sigmaV = 0.03 * -0.3 + 0.001;
    float g_sigmaY = 3;
    float w, h, miplevels;
    texSSAO.GetDimensions(0, w, h, miplevels);

    float2 g_pixelSize = (1.0f / w, 1.0f / h);

    const float c_halfSamplesX = 4.;
    const float c_halfSamplesY = 4.;

    float total = 0.0;
    float ret = 0.0;

    float2 pivot = texSSAO.Sample(smp, uv);
    
    for (float iy = -c_halfSamplesY; iy <= c_halfSamplesY; iy++)
    {
        float fy = Gaussian(g_sigmaY, iy);
        float offsety = iy * g_pixelSize.y;

        for (float ix = -c_halfSamplesX; ix <= c_halfSamplesX; ix++)
        {
            float fx = Gaussian(g_sigmaX, ix);
            float offsetx = ix * g_pixelSize.x;
            
            float2 value = texSSAO.Sample(smp, uv + float2(offsetx, offsety));
                        
            float fv = Gaussian(g_sigmaV, abs(value.y - pivot.y));
            
            total += fx * fy * fv;
            ret += fx * fy * fv * value.r;
        }
    }
        
    return ret / total;
}
  
float4 BilateralUpsampling(float2 uv)
{
	//�t���T�C�Y�e�N�X�`���̑傫���擾
    float texW, texH, texMiplevels;
    tex.GetDimensions(0, texW, texH, texMiplevels);
	//�@�@�@�@�@�@�@�@0 1  0 1  0 1
	//���݂̃e�N�Z��������|����|����  ���擾
	//�@�@�@�@�@�@�@�@����|����|����
	//�@�@�@�@�@�@�@�@2 3  2 3  2 3
    int nTexcel = (int) (uv.x * texW) % 2 + (((int) (uv.y * texH) % 2) * 2);

    float4 returnColor = { 1, 1, 1, 1 };

	//�k���e�N�X�`���̑傫���擾
    float w, h, miplevels;
    halfNormTex.GetDimensions(0, w, h, miplevels);

	//�k���e�N�X�`���̃e�N�Z���������v�Z
    float dx = 1.0f / w;
    float dy = 1.0f / h;

	//�o�C���e�����t�B���^�����̏d�݂�ݒ�
    float4 vBilinearWeights[4] =
    {
			  // 0   1    2    3
        float4(9.0f / 16.0f, 3.0f / 16.0f, 3.0f / 16.0f, 1.0f / 16.0f), //0
    	float4(3.0f / 16.0f, 9.0f / 16.0f, 1.0f / 16.0f, 3.0f / 16.0f), //1
    	float4(3.0f / 16.0f, 1.0f / 16.0f, 9.0f / 16.0f, 3.0f / 16.0f), //2
    	float4(1.0f / 16.0f, 3.0f / 16.0f, 3.0f / 16.0f, 9.0f / 16.0f), //3
    };

	//�k���o�b�t�@��4�e�N�Z�����I�t�Z�b�g��ݒ�
    float2 uvOffset[4] =
    {
		{ dx * -0.5f, dy * -0.5f},
		{ dx *  0.5f, dy * -0.5f},
		{ dx * -0.5f, dy *  0.5f},
		{ dx *  0.5f, dy *  0.5f},
    };

	//�@���ɑ΂��Ă̏d�݂�������
    float vNormalWeights[4] =
    {
       0,0,0,0
    };
	//�@���e�N�X�`���̃t���T�C�Y����l���T���v�����O
    float4 vNormalHiRes = normTex.Sample(smp, uv);
	//�@���k���e�N�X�`���̌���UV����4�s�N�Z�����ɂ���
    for (int i = 0; i < 4;i++)
    {
    	//�k���e�N�X�`������l���T���v�����O
        float4 vNormalCoarse = halfNormTex.Sample(smp, uv + uvOffset[i]);
    	//�d�݂̌v�Z
        vNormalWeights[i] = dot(vNormalCoarse, vNormalHiRes);
        vNormalWeights[i] = pow(vNormalWeights[i], 32);
    }

	//�[�x�l�ɑ΂��Ă̏d�݂�������
    float vDepthWeights[4] =
    {
        0,0,0,0
    };
	//�[�x�e�N�X�`���̃t���T�C�Y����l���T���v�����O
    float fDepthHiRes = depthTex.Sample(smp, uv);
	//�[�x�k���e�N�X�`���̌���UV����4�s�N�Z�����ɂ���
    for (int i = 0; i < 4; i++)
    {
    	//�k���e�N�X�`������l���T���v�����O
        float fDepthCoarse = halfDepthTex.Sample(smp, uv + uvOffset[i]);
    	//�d�݂̌v�Z
        float fDepthDiff = fDepthHiRes - fDepthCoarse;
        vDepthWeights[i] = 1.0f / (2.22045e-016 + abs(fDepthDiff));
    }

	//�ŏI�I�ȏd�݂�������
    float fTotalWeight = 0;
    float vUpsampled = 0;
    for (int i = 0; i < 4; i++)
    {
    	//4�e�N�Z�����̖@���A�[�x�A�t�B���^�̏d�݂���Z
        float fWeight = vNormalWeights[i] * vDepthWeights[i] * vBilinearWeights[nTexcel][i];
        fTotalWeight += fWeight;
        vUpsampled += texSSAO.Sample(smp, uv + uvOffset[i]) * fWeight;
    }
	//0�`1�̊ԂɎ��߂邽�߂ɐ��K��
    returnColor = vUpsampled /= fTotalWeight;
    return returnColor;
}
float4 PSmain(VSOutput input) : SV_TARGET
{
	//�k���o�b�t�@���̂܂�
    //float ssao = texSSAO.Sample(smp, input.uv);
    //float4 returnColor = float4(ssao, ssao, ssao, 1.0f);
    //return returnColor;

	//�o�C���e�����t�B���^�[�̂�
    //float filtered = JoinedBilateralGaussianBlur(input.uv);
    //const float4 ssaoColor = float4(float3(filtered, filtered, filtered), 1.0);

	//�o�C���e�����A�b�v�T���v�����O
    float4 col = tex.Sample(smp, input.uv);
    const float4 ssaoColor = BilateralUpsampling(input.uv);
    float4 returnColor = float4(col.rgb * ssaoColor.rgb, col.a);
    return returnColor;
}


