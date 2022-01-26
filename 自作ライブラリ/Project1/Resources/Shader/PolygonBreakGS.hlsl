
#include "Collision.hlsli"
#include "PolygonBreak.hlsli"
#include"PerlinNoise.hlsli"


float3 rotate(in float3 vec, in float3 axis, in float val)
{
    float4 tmp = float4(vec.xyz, 0);

    const float3 a = axis.xyz;
    const float s = sin(val);
    const float c = cos(val);
    const float r = 1.0 - c;

    const float4x4 tmpMatrix = float4x4(
					a.x * a.x * r + c,         a.y * a.x * r + a.z * s,   a.z * a.x * r - a.y * s, 0,
					a.x * a.y * r - a.z * s,   a.y * a.y * r + c,         a.z * a.y * r + a.x * s, 0,
					a.x * a.z * r + a.y * s,   a.y * a.z * r - a.x * s,   a.z * a.z * r + c,       0,
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
//float CheckBreakTarget(float3 checkPoint)
//{
//	//���[���h�s��̋t�s����v�Z
//    const matrix inverseMat = inverse(world);
//    //���𒆐S�_(���[�J�����W�ɕϊ�)
//    const float3 breakPoint = mul(float4(BreakPoint, 0), inverseMat).xyz;

//    const float3 btc = breakPoint - checkPoint;
	
//    const float length = sqrt(btc.x * btc.x + btc.y * btc.y + btc.z * btc.z);

//    const float result = lerp(1 /*- smoothstep(BreakRadius / 6, BreakRadius*3, length*3)*/, 0, step(BreakRadius, length));
	
//    return result;
//}


//�J�������璍���_�܂ł̊Ԃɂ��邩����(�J�v�Z��)
float CheckInCameraTarget(float3 checkPos)
{
    const float3 camToTargetVec = normalize(TargetPos - cameraPos);
	
    const float3 camPos = cameraPos;
    const float3 targetPos = TargetPos - camToTargetVec * CapsuleRadius * 0.80f;

    const float3 vStartToEnd = targetPos - camPos;
	
    float3 n = vStartToEnd;
    n = normalize(n);
	
    const float t = dot(float3(checkPos - camPos), n);

    const float3 vPsPn = n * t;
    const float3 posPn = vPsPn + camPos;
    
    const float lengthRate = t / length(vStartToEnd);

    const float distance = lerp(length(float3(checkPos - camPos)), lerp(length(float3(checkPos - posPn)), length(float3(checkPos - targetPos)), step(1.0001f, lengthRate)), step(0, lengthRate));
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
    const float3 vec1 = input[1].worldpos - input[0].worldpos;
    const float3 vec2 = input[2].worldpos - input[0].worldpos;
	
    float r = 2.0 * (random(float2(random(center.xy), random(center.yx))) - 0.5);
    const float3 r3 = r.xxx;
    const float3 randNormal = float3(random(float2(random(center.xx), random(center.zx))) - 0.5, random(float2(random(center.xy), random(center.xz))) - 0.5, random(float2(random(center.yy), random(center.yz))) - 0.5);
    const float3 normal = normalize(cross(vec1, vec2) + randNormal);
    const float3 randNormal2 = float3(random(float2(random(randNormal.xx), random(randNormal.zx))) - 0.5, random(float2(random(randNormal.xy), random(randNormal.xz))) - 0.5, random(float2(random(randNormal.yy), random(randNormal.yz))) - 0.5);

    const float randPoint = abs(random(randNormal2.xy) / 2);

    Triangle tri = TriangleInit;
    tri.p0 = input[0].worldpos.xyz;
    tri.p1 = input[1].worldpos.xyz;
    tri.p2 = input[2].worldpos.xyz;
    tri.normal = normalize(cross(input[1].worldpos.xyz - input[0].worldpos.xyz, input[2].worldpos.xyz - input[0].worldpos.xyz));
    Ray ray = RayInit;
    ray.start = cameraPos;
    ray.dir = normalize(TargetPos - cameraPos);
    ray.dir.y = lerp(-0.0001, ray.dir.y, step(ray.dir.y, -0.0001));
	
    const float3 checkPos = /*(input[0].worldpos.xyz + input[1].worldpos.xyz + input[2].worldpos.xyz) / 3;*/CulcCheckPoint(ray,tri);
	
    const float cameraDestruct = lerp(0, CheckInCameraTarget(checkPos), step(0.5, _OnCameraBreak));
	
    const float destruction = clamp(0, 1, lerp(_Destruction, EaseInOutQuart(0, 1.0f, 600, 600 * smoothstep(randPoint, randPoint + 0.5f, _Destruction)), step(0.5, _OnEasing)) /** lerp(1, CheckBreakTarget(center), step(0.5, _OnTargetBreak))*/ + cameraDestruct);


    GSOutput outputs[4];
    for (uint i = 0; i < 3; i++)
    {
        DS_OUTPUT v = input[i];
        GSOutput o;

       //  UNITY_SETUP_INSTANCE_ID(v);
       //UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);
        // center�ʒu���N�_�ɃX�P�[����ω��B
        v.svpos.xyz = (v.svpos.xyz - center) * (1.0 - smoothstep(0.1f, 1.0f, destruction) * _ScaleFactor) + center;
        // center�ʒu���N�_�ɁA������p���ĉ�]��ω�
        v.svpos.xyz = rotate(v.svpos.xyz - center, normal, r3 * smoothstep(0.1f, 1.0f, destruction) * _RotationFactor).xyz + center;

        // �@�������Ɉʒu��ω�
        //v.svpos.xyz += normal * destruction * _PositionFactor * (r3 + 1.0);
        v.svpos.xyz += normal * smoothstep(0.2f, 0.5f, destruction) * _PositionFactor * (r3 + 1.0) + normalize(randNormal2) * smoothstep(0.5f, 1.0f, destruction) * _PositionFactor * (r3 + 1.0);

        // �Ō�ɁA�C���������_�ʒu���ˉe�ϊ��������_�����O�p�ɕϊ�
        o.svpos = mul(mul(viewproj, world), v.svpos);
        o.shadowPos = mul(mul(lightCameraVP, world), v.svpos);

        const float4 wnormal = normalize(mul(world, float4(v.normal, 0)));

        o.normal = wnormal.xyz;
        o.uv = v.uv;
        //float4 wpos = mul(world, v.worldpos);
        //o.worldpos = wpos;
        o.worldpos = v.worldpos;
        //UNITY_TRANSFER_FOG(o, o.vertex);
        o.destruction = destruction;
        outputs[i] = o;
        output.Append(o);
    }
}
