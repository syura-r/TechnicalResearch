static const float PI = 3.141592654f;
static float3 N;
	//�A���x�h
static float3 baseColor = float3(1,1,1);
	//�����x
static float metalness = 1.0f;
	//���ʔ��ˋ��x
static float specular = 1.0f;
	//�e��
static float roughness = 0.4f;

//Schlick�ɂ��ߎ�
//f0��f90�̒l��(1-cosine)��5���lerp����
//f0 : ���������ɓ��˂����Ƃ��̔��˗�
//f90 : �������s�ɓ��˂����Ƃ��̔��˗�
//cosine ; 2�x�N�g���̂Ȃ��e�̃R�T�C��(���ϒl)
float SchlickFresnel(float f0, float f90, float cosine)
{
    float m = saturate(1 - cosine);
    float m2 = m * m; //�������̂���pow���g��Ȃ�
    float m5 = m2 * m2 * m;
    return lerp(f0, f90, m5);
}


float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
    float m = saturate(1 - cosine);
    float m2 = m * m;
    float m5 = m2 * m2 * m;
    return lerp(f0, f90, m5);
}


//UE4��GGX���z
//alpha : roughness��2��
//NdotH : �@���ƃn�[�t�x�N�g���̓���
float DistributionGGX(float alpha, float NdotH)
{
    const float alpha2 = alpha * alpha;
    float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
    return alpha2 / (PI * t * t);
}

//�f�B�Y�j�[�̃t���l���v�Z
float3 DisneyFresnel(float LdotH)
{
	//F��(�t���l�� : Fresnel)
	//�P�x
    float luminance = 0.3f * baseColor.r + 0.6f * baseColor.g + 0.1f * baseColor.b;
	//�F����
    float3 tintColor = baseColor / luminance;
	//������̋��ʔ��ːF���v�Z
    float3 nonMetalColor = specular * 0.08f * tintColor;
	//metalness�ɂ��F��� �����̏ꍇ�̓x�[�X�J���[
    float3 specularColor = lerp(nonMetalColor, baseColor, metalness);
	//NdotH�̊�����SchlickFresnel���
    return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

//UE4��Smith���f��
float GeometricSmith(float cosine)
{
    float k = (roughness + 1.0f);
    k = k * k / 8.0f;

    return cosine / (cosine * (1.0f - k) + k);
}

//���ʔ��˂̌v�Z
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	//D��(���z:Distribution)
    float Ds = DistributionGGX(roughness * roughness, NdotH);

	//F��(�t���l��:Fresnel)
    float3 Fs = DisneyFresnel(LdotH);

	//G��(�􉽌���:Geometry attenuation)
    float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	//m��(����)
    float m = 4.0f * NdotL * NdotV;

	//�������ċ��ʔ��˂̐F�𓾂�
    return Ds * Fs * Gs / m;
}

//�o�������˕��z�֐�
float3 BRDF(float3 L, float3 V)
{
	//�@���ƃ��C�g�����̓���
    float NdotL = dot(N, L);
	//�@���ƃJ���������̓���
    float NdotV = dot(N, V);
	//�ǂ��炩��90�x�ȏ�ł���ΐ^������Ԃ�
    if (NdotL < 0 || NdotV < 0)
    {
        return float3(0, 0, 0);
    }
    //���C�g�����ƃJ���������̒���(�n�[�t�x�N�g��)
    float3 H = normalize(L + V);
	//�@���ƃn�[�t�x�N�g���̓���
    float NdotH = dot(N, H);
	//���C�g�ƃn�[�t�x�N�g���̓���
    float LdotH = dot(L, H);
	
	//�g�U���˗�
    float diffuseReflectance = 1.0f / PI;
    float energyBias = 0.5f * roughness;
	
	//���ˊp��90�x�̏ꍇ�̊g�U���˗�
    float Fd90 = energyBias + 2 * LdotH * LdotH * roughness;
	//�����Ă������̊g�U���˗�
    float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//�o�Ă������̊g�U���˗�
    float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	//�����ďo�Ă����܂ł̊g�U���˗�
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
    float Fd = FL * FV * energyFactor;
	
	//�g�U���ˍ�
    float3 diffuseColor = diffuseReflectance * Fd * baseColor * (1 - metalness);
    //���ʔ��ˍ�
    float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);
	//�g�U���˂Ƌ��ʔ��˂̍��v�ŐF�����܂�
    return diffuseColor + specularColor;
}
