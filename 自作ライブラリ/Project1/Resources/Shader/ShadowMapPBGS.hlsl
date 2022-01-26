
#include "ShadowMap.hlsli"
#include"PerlinNoise.hlsli"

cbuffer polygonBreak : register(b1)
{
    float _Destruction; //����x����
    float _ScaleFactor; //�X�P�[���̕ω���
    float _PositionFactor; //�|�W�V�����̕ω���
    float _RotationFactor; //��]�̕ω���
    uint _Tessellation; //�|���S�������x
    uint _OnEasing; //�C�[�W���O���邩�ǂ���
    uint _OnTargetBreak; //����̈ʒu�̂ݕ������邩�ǂ���
    uint _OnCameraBreak;
    float3 BreakPoint; //���𒆐S�_
    float BreakRadius; //���𔼌a
    float3 TargetPos; //�J�����̒����_
    float CapsuleRadius; //�J�������璍���_�܂ł̃J�v�Z�����a
    uint _On4Vertex;

}
matrix inverse(matrix m)
{
    const float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    const float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    const float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    const float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    const float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    const float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    const float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    const float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    const float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    const float idet = 1.0f / det;

    matrix ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

float3 rotate(float3 vec,float3 axis, float val)
{
    float4 tmp = float4(vec.xyz, 0);

    const float3 a = (axis.xyz != 0) ? axis.xyz : 0;
    const float s = sin(val);
    const float c = cos(val);
    const float r = 1.0 - c;

    const float4x4 tmpMatrix = float4x4(
					a.x * a.x * r + c, a.y * a.x * r + a.z * s, a.z * a.x * r - a.y * s, 0,
					a.x * a.y * r - a.z * s, a.y * a.y * r + c, a.z * a.y * r + a.x * s, 0,
					a.x * a.z * r + a.y * s, a.y * a.z * r - a.x * s, a.z * a.z * r + c, 0,
					0, 0, 0, 1
					);
    tmp = mul(tmpMatrix, tmp);
    return tmp;
}


float EaseInOutQuart(const float start, const float end, const int count, const float time)
{
    float t = time;
    t /= count;
    return lerp(start, end, 8 * t * t * t * t * step(0.5f, 1 - t) + (1 - pow(-2 * t + 2, 4) / 2) * step(0.5, t));

}

//����Ώۂ��̔���֐�(��)
float CheckBreakTarget(float3 checkPoint)
{
	//���[���h�s��̋t�s����v�Z
    const matrix inverseMat = inverse(world);
    //���𒆐S�_(���[�J�����W�ɕϊ�)
    const float3 breakPoint = mul(float4(BreakPoint, 0), inverseMat).xyz;

    const float3 btc = breakPoint - checkPoint;
	
    const float length = sqrt(btc.x * btc.x + btc.y * btc.y + btc.z * btc.z);

    const float result = lerp(1 /*- smoothstep(BreakRadius / 6, BreakRadius*3, length*3)*/, 0, step(BreakRadius, length));
	
    return result;
}
//�J�������璍���_�܂ł̊Ԃɂ��邩����(�J�v�Z��)
float CheckInCameraTarget(float3 checkPoint)
{
	//���[���h�s��̋t�s����v�Z
    const matrix inverseMat = inverse(world);
    //�J�����̈ʒu(���[�J�����W�ɕϊ�)
    const float3 camPos = mul(float4(cameraPos, 0), inverseMat).xyz;
    //�J�����̒����_�ʒu(���[�J�����W�ɕϊ�)
    const float3 targetPos = mul(float4(TargetPos, 0), inverseMat).xyz;

    const float3 vStartToEnd = targetPos - camPos;

    float3 n = vStartToEnd;
    n = normalize(n);
	
    const float t = dot(float3(checkPoint - camPos), n);

    const float3 vPsPn = n * t;
    const float3 posPn = vPsPn + camPos;
    
    const float lengthRate = t / length(vStartToEnd);

    const float distance = lerp(length(float3(checkPoint - camPos)), lerp(length(float3(checkPoint - posPn)), CapsuleRadius, step(1.0001f, lengthRate)), step(0, lengthRate));

    const float result = lerp(1 - smoothstep(CapsuleRadius / 6, CapsuleRadius, distance), 0, step(CapsuleRadius, distance));
	
    return result;

}

[maxvertexcount(12)]
void GSmain(
	triangle DS_OUTPUT input[3] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	// �|���S���̒��S���v�Z�B
    const float3 center = (input[0].svpos + input[1].svpos + input[2].svpos) / 3;

    // �|���S���̕Ӄx�N�g�����v�Z���A�|���S���̖@�����v�Z����B
    const float3 vec1 = input[1].svpos - input[0].svpos;
    const float3 vec2 = input[2].svpos - input[0].svpos;

    //const float destruction = _Destruction;
	
    float r = 2.0 * (random(float2(random(center.xy), random(center.yx))) - 0.5);
    const float3 r3 = r.xxx;
    const float3 randNormal = float3(random(float2(random(center.xx), random(center.zx))) - 0.5, random(float2(random(center.xy), random(center.xz))) - 0.5, random(float2(random(center.yy), random(center.yz))) - 0.5);
    const float3 normal = normalize(cross(vec1, vec2) + randNormal);
    const float3 randNormal2 = float3(random(float2(random(randNormal.xx), random(randNormal.zx))) - 0.5, random(float2(random(randNormal.xy), random(randNormal.xz))) - 0.5, random(float2(random(randNormal.yy), random(randNormal.yz))) - 0.5);

    const float randPoint = abs(random(randNormal2.xy) / 2);
	
    const float destruction = clamp(0, 1, lerp(_Destruction, EaseInOutQuart(0, 1.0f, 600, 600 * smoothstep(randPoint, randPoint + 0.5f, _Destruction)), step(0.5, _OnEasing)) * lerp(1, CheckBreakTarget(center), step(0.5, _OnTargetBreak)));

    for (uint i = 0; i < 3; i++)
    {
        DS_OUTPUT v = input[i];
        GSOutput o;

       //  UNITY_SETUP_INSTANCE_ID(v);
       //UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);
        // center�ʒu���N�_�ɃX�P�[����ω������܂��B
        v.svpos.xyz = (v.svpos.xyz - center) * (1.0 - smoothstep(0.1f, 1.0f, destruction) * _ScaleFactor) + center;
        // center�ʒu���N�_�ɁA������p���ĉ�]��ω������܂��B
        v.svpos.xyz = rotate(v.svpos.xyz - center, center, r3 * smoothstep(0.1f, 1.0f, destruction) * _RotationFactor).xyz + center;

        // �@�������Ɉʒu��ω������܂�
        //v.svpos.xyz += normal * destruction * _PositionFactor * (r3 + 1.0);
        v.svpos.xyz += normal * smoothstep(0.2f, 0.5f, destruction) * _PositionFactor * (r3 + 1.0) + normalize(randNormal2) * smoothstep(0.5f, 1.0f, destruction) * _PositionFactor * (r3 + 1.0);

        // �Ō�ɁA�C���������_�ʒu���ˉe�ϊ��������_�����O�p�ɕϊ����܂��B
        o.svpos = mul(mul(viewproj, world), v.svpos);

        output.Append(o);
    }
    //}
    output.RestartStrip();
}
