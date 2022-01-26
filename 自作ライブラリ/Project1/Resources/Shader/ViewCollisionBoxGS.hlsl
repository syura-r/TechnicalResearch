#include "ViewCollisionBoxHeader.hlsli"

static const matrix MatInitialize =
{
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
};
matrix Rotation(float3 rot)
{
    const matrix rotationX =
    {
        1, 0, 0, 0,
			0, cos(rot.x), -sin(rot.x), 0,
			0, sin(rot.x), cos(rot.x), 0,
			0, 0, 0, 1
    };
    const matrix rotationY =
    {
        cos(rot.y), 0, sin(rot.y), 0,
			0, 1, 0, 0,
			-sin(rot.y), 0, cos(rot.y), 0,
			0, 0, 0, 1
    };
    const matrix rotationZ =
    {
        cos(rot.z), -sin(rot.z), 0, 0,
			sin(rot.z), cos(rot.z), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
    };
    matrix rotation = MatInitialize;
    rotation = mul(rotationZ, rotation);
    rotation = mul(rotationX, rotation);
    rotation = mul(rotationY, rotation);
    return rotation;
}
[maxvertexcount(26)]
void GSmain(
	point VSOutput input[1] : SV_POSITION, 
	inout LineStream<GSOutput> output
)
{
    const float3 scale = input[0].scale * 2;
    const float4 center = input[0].center;
    const float4 distance = input[0].objCenter - center;//Collider�̒��S�ʒu����I�u�W�F�N�g�̒��S�ʒu�܂ł̋���
	// �Z���^�[����̃I�t�Z�b�g�e�[�u��
    float4 offset_array[8] =
    {
        float4(-0.5f, +0.5f, -0.5f, 0), ///*0*/�����O
		float4(+0.5f, +0.5f, -0.5f, 0), ///*1*/�E���O
		float4(+0.5f, +0.5f, +0.5f, 0), ///*2*/�E�㉜(MaxPoint)
		float4(-0.5f, +0.5f, +0.5f, 0), ///*3*/���㉜
		float4(-0.5f, -0.5f, -0.5f, 0), ///*4*/������O(MinPoint)
		float4(+0.5f, -0.5f, -0.5f, 0), ///*5*/�E����O
		float4(+0.5f, -0.5f, +0.5f, 0), ///*6*/�E����
		float4(-0.5f, -0.5f, +0.5f, 0)  ///*7*/������
    };

    const matrix colRotation = Rotation(input[0].colRotation);
    const matrix objRotation = Rotation(input[0].rotation);

    for (int i = 0; i < 8;i++)
    {
        const float4 offset = offset_array[i] * float4(scale, 1);
        offset_array[i] = mul(colRotation, offset);
        offset_array[i] -= distance;
        offset_array[i] = mul(objRotation, offset_array[i]);
        offset_array[i] += distance;
    }
	//xz���ʂ�2�ʕ�
    for (uint i = 0; i < 2; i++)
    {
        for (uint j = 0; j < 4; j++)
        {
            const float4 offset1 = offset_array[j + (i * 4)];
            const float4 offset2 = offset_array[lerp(j + 1 + (i * 4), i * 4, step(2.5f, j))];

            GSOutput p1, p2;
            p1.pos = center + offset1;
            p1.pos = mul(mat, p1.pos);
            p2.pos = center + offset2;
            p2.pos = mul(mat, p2.pos);
            p1.color = float3(1, 0, 0);
            p2.color = float3(1, 0, 0);
            output.Append(p1);
            output.Append(p2);
            output.RestartStrip();
        }
    }
	//�c��̏c�̐���4��
    for (uint j = 0; j < 4; j++)
    {
        const float4 offset1 = offset_array[j] ;
        const float4 offset2 = offset_array[j+4];

        GSOutput p1, p2;
        p1.pos = center + offset1;
        p1.pos = mul(mat, p1.pos);
        p2.pos = center + offset2;
        p2.pos = mul(mat, p2.pos);
        p1.color = float3(1, 0, 0);
        p2.color = float3(1, 0, 0);
        output.Append(p1);
        output.Append(p2);
        output.RestartStrip();
    }

	//�����Ă�����̐���
    GSOutput p1, p2;
    float4 offset1 = { 0, 0, 0.5f, 0 };
    offset1 *= float4(scale, 1);
    float4 offset2 = { 0, 0, 1, 0 };
    offset2 += offset1;
    offset1 = mul(colRotation, offset1);
    offset1 -= distance;
    offset1 = mul(objRotation, offset1);
    offset1 += distance;
    offset2 = mul(colRotation, offset2);
    offset2 -= distance;
    offset2 = mul(objRotation, offset2);
    offset2 += distance;
    p1.pos = center + offset2;
    p1.pos = mul(mat, p1.pos);
    p2.pos = center + offset1;
    p2.pos = mul(mat, p2.pos);
    p1.color = float3(0,0,1);
    p2.color = float3(0,0,1);
    output.Append(p1);
    output.Append(p2);
    output.RestartStrip();

}