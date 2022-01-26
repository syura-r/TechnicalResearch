#include "Collision.h"
#include"ImGui.h"
#include <d3d12.h>
#include <string>
#include <utility>
Window* Collision::window = nullptr;
using namespace DirectX;



float Collision::CheckMostLongAxis(const XMVECTOR& vec)
{
	//3���������ꂼ��̒������v�Z
	float absX = fabs(vec.m128_f32[0]);
	float absY = fabs(vec.m128_f32[1]);
	float absZ = fabs(vec.m128_f32[2]);
	float result = 0;
	if(absX>absY)
	{
		if(absX>absZ)
		{
			result = vec.m128_f32[0];
		}
		else
		{
			result = vec.m128_f32[2];
		}
	}
	else
	{
		if (absY > absZ)
		{
			result = vec.m128_f32[1];
		}
		else
		{
			result = vec.m128_f32[2];
		}
	}
	return result;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR & point, const Triangle & triangle, DirectX::XMVECTOR * closest)
{
	//point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	float d1 = XMVector3Dot(p0_p1, p0_pt).m128_f32[0];
	float d2 = XMVector3Dot(p0_p2, p0_pt).m128_f32[0];

	if (d1 <= 0.0f&&d2 <= 0.0f)
	{
		//p0���ŋߒl
		*closest = triangle.p0;
		return;
	}

	//point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	float d3 = XMVector3Dot(p0_p1, p1_pt).m128_f32[0];
	float d4 = XMVector3Dot(p0_p2, p1_pt).m128_f32[0];

	if (d3 > -0.0f&&d4 <= d3)
	{
		//p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	//point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0 && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	//point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	float d5 = XMVector3Dot(p0_p1, p2_pt).m128_f32[0];
	float d6 = XMVector3Dot(p0_p2, p2_pt).m128_f32[0];
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	// point�͖ʗ̈�̒��ɂ���Bclosest���d�S���W��p���Čv�Z����
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;

}
bool Collision::CheckSphereBox(const Sphere& sphere, const Box& box, XMVECTOR* inter, XMVECTOR* reject)
{

	XMVECTOR rejectVec = XMVectorZero();
	float sqDistance = 0.0f;
	float d;

	d = sphere.center.m128_f32[0];
	if (d < box.minPosition.x)
	{
		sqDistance += (d - box.minPosition.x) * (d - box.minPosition.x);
		rejectVec.m128_f32[0] = box.minPosition.x - (d + sphere.radius);
	}
	if (d > box.maxPosition.x)
	{
		sqDistance += (d - box.maxPosition.x) * (d - box.maxPosition.x);
		rejectVec.m128_f32[0] =  box.maxPosition.x - (d - sphere.radius);
	}

	d = sphere.center.m128_f32[1];
	if (d < box.minPosition.y)
	{
		sqDistance += (d - box.minPosition.y) * (d - box.minPosition.y);
		rejectVec.m128_f32[1] = box.minPosition.y - (d + sphere.radius);
	}
	if (d > box.maxPosition.y)
	{
		sqDistance += (d - box.maxPosition.y) * (d - box.maxPosition.y);
		rejectVec.m128_f32[1] = box.maxPosition.y - (d - sphere.radius);

	}
	d = sphere.center.m128_f32[2];
	if (d < box.minPosition.z)
	{
		sqDistance += (d - box.minPosition.z) * (d - box.minPosition.z);
		rejectVec.m128_f32[2] = box.minPosition.z - (d + sphere.radius);
	}
	if (d > box.maxPosition.z)
	{
		sqDistance += (d - box.maxPosition.z) * (d - box.maxPosition.z);
		rejectVec.m128_f32[1] = box.maxPosition.z - (d - sphere.radius);
	}

	if (sqDistance > sphere.radius * sphere.radius)
		return false;


	if (reject)
	{
		*reject = rejectVec;
	}
	if(inter)
	{
		//�����Ă���̂�...?
		*inter = (sphere.center + rejectVec) +  XMVector3Normalize(-rejectVec) * sphere.radius;
	}
	return true;


}
bool Collision::CheckAABBAABB(const Box& box1, const Box& box2, XMVECTOR* inter, XMVECTOR* reject)
{
	if (box2.maxPosition.x >= box1.minPosition.x && box2.minPosition.x <= box1.maxPosition.x &&
		box2.maxPosition.y >= box1.minPosition.y && box2.minPosition.y <= box1.maxPosition.y &&
		box2.maxPosition.z >= box1.minPosition.z && box2.minPosition.z <= box1.maxPosition.z)
	{
		if (inter)
		{
			Vector3 collPos = {};
			collPos.x = box2.maxPosition.x - ((box2.maxPosition.x - box1.minPosition.x) / 2);
			collPos.y = box2.maxPosition.y - ((box2.maxPosition.y - box1.minPosition.y) / 2);
			collPos.z = box2.maxPosition.z - ((box2.maxPosition.z - box1.minPosition.z) / 2);
			*inter = XMVectorSet(collPos.x, collPos.y, collPos.z, 1);
		}
		if(reject)
		{
			//�l����(�ړ��x�N�g���Ȃ��Ɩ�������Ȃ��c�H)
		}
		return true;
	}
	return false;
}

bool Collision::CheckSphere2Sphere(const Sphere & sphereA, const Sphere & sphereB, DirectX::XMVECTOR * inter, DirectX::XMVECTOR * reject)
{
	// ���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
	float dist = XMVector3LengthSq(sphereA.center - sphereB.center).m128_f32[0];

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2) {
		if (inter) {
			// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = XMVectorLerp(sphereA.center, sphereB.center, t);
		}
		//�����o���x�N�g�����v�Z
		if (reject)
		{
			float rejectLen = sphereA.radius + sphereB.radius - sqrt(dist);
			*reject = XMVector3Normalize(sphereA.center - sphereB.center);
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Plane(const Sphere & sphere, const Plane & plane, DirectX::XMVECTOR * inter)
{
	// ���W�n�̌��_���狅�̒��S���W�ւ̋�������
	// ���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = XMVector3Dot(sphere.center, plane.normal).m128_f32[0] - plane.distance;
	// �����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius)	return false;

	// �[����_���v�Z
	if (inter)
	{
		// ���ʏ�̍Đڋߓ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}
	return true;
}

bool Collision::CheckSphere2Triangle(const Sphere & sphere, const Triangle & triangle, DirectX::XMVECTOR * inter, DirectX::XMVECTOR * reject)
{
	XMVECTOR p;
	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// �_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	// �����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	float distanceSquare = XMVector3Dot(v, v).m128_f32[0];
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (distanceSquare > sphere.radius * sphere.radius)	return false;
	// �[����_���v�Z
	if (inter) {
		// �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}
	//�����o���x�N�g�����v�Z
	if (reject)
	{
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal*rejectLen;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray & lay, const Plane & plane, float * distance, DirectX::XMVECTOR * inter)
{
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl

	float d1 = XMVector3Dot(plane.normal, lay.dir).m128_f32[0];
	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) {
		return false;
	}

	float d2 = XMVector3Dot(plane.normal, lay.start).m128_f32[0];
	float t = (plane.distance - d2) / d1;

	if (t < 0) return false;

	// ��������������
	if (distance) {
		*distance = t;
	}

	// ��_���v�Z
	if (inter) {
		*inter = lay.start + t * lay.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray & lay, const Triangle & triangle, float * distance, DirectX::XMVECTOR * inter)
{
	// �O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�	
	if (!CheckRay2Plane(lay, plane, distance, &interPlane)) {
		return false;
	}
	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ

	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl
	XMVECTOR m;
	// ��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// ��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// ��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	if (inter) {
		*inter = interPlane;
	}

	// �����Ȃ̂ŁA�������Ă���
	return true;
}

bool Collision::CheckRay2Sphere(const Ray & lay, const Sphere & sphere, float * distance, DirectX::XMVECTOR * inter)
{
	XMVECTOR m = lay.start - sphere.center;
	float b = XMVector3Dot(m, lay.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	// lay�̎n�_��sphere�̊O���ɂ���(c > 0)�Alay��sphere���痣��Ă���������
	// �����Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f) {
		return false;
	}

	float discr = b * b - c;
	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) {
		return false;
	}

	// ���C�͋��ƌ������Ă���B
	// ��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0) t = 0.0f;

	if (distance) {
		*distance = t;
	}

	if (inter) {
		*inter = lay.start + t * lay.dir;
	}

	return true;
}
bool Collision::CheckRay2OBB(const Ray& ray,const Box& obb, float* distance, DirectX::XMVECTOR* inter)
{
	Plane plane;
	float scale[3] = { obb.scale.x ,obb.scale.y ,obb.scale.z };
	bool result = false;
	float resultDistance = D3D12_FLOAT32_MAX;
	XMVECTOR resultInter = {};
	int resultInt = 0;
	//OBB6�ʕ�,�ʂƃ��C�̔�����s��
	for (int i = 0; i < 6; i++)
	{
		XMVECTOR otherDir[2];
		//i��0,3�̎���X����1,4�̂Ƃ���Y����2,5�̂Ƃ���Z����
		int val2 = i % 3;
		if (val2 == 0)
		{
			otherDir[0] = obb.normaDirect[1] * obb.scale.y;
			otherDir[1] = obb.normaDirect[2] * obb.scale.z;
		}
		else if (val2 == 1)
		{
			otherDir[0] = obb.normaDirect[0] * obb.scale.x;
			otherDir[1] = obb.normaDirect[2] * obb.scale.z;
		}
		else
		{
			otherDir[0] = obb.normaDirect[0] * obb.scale.x;
			otherDir[1] = obb.normaDirect[1] * obb.scale.y;
		}

		//0-2�͎��ɑ΂��Đ��̕����A3-5�͕��̕���
		int val = i / 3;
		plane.normal = obb.normaDirect[i - 3 * val] * (1 - 2*val);

		XMVECTOR p0 = otherDir[0] + otherDir[1] + obb.normaDirect[i - 3 * val] * (1 - 2 * val) * scale[i - 3 * val] + obb.center;
		
		plane.distance = XMVector3Dot(plane.normal ,p0).m128_f32[0];
		float dis;
		XMVECTOR interPlane;
		// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�	
		if (!CheckRay2Plane(ray, plane, &dis, &interPlane)) {
			continue;
		}
		//�Փˋ����������̂��̂�菬���������������������
		if (resultDistance > dis)
		{
			resultDistance = dis;
			resultInter = interPlane;
			resultInt = i;
		}
		result = true;
	}
	if (!result)
		return false;

	//0-2�͎��ɑ΂��Đ��̕����A3-5�͕��̕���
	int val = resultInt / 3;
	//i��0,3�̎���X����1,4�̂Ƃ���Y����2,5�̂Ƃ���Z����
	int val2 = resultInt % 3;
	auto normal = obb.normaDirect[resultInt - 3 * val] * (1 - 2 * val);
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl
	float normalScale = {};
	XMVECTOR otherDir[2];

	if(val2 == 0)
	{
		normalScale = obb.scale.x;
		otherDir[0] = obb.normaDirect[1] * obb.scale.y;
		otherDir[1] = obb.normaDirect[2] * obb.scale.z;
	}
	else if(val2 == 1)
	{
		normalScale = obb.scale.y;
		otherDir[0] = obb.normaDirect[0] * obb.scale.x;
		otherDir[1] = obb.normaDirect[2] * obb.scale.z;
	}
	else
	{
		normalScale = obb.scale.z;
		otherDir[0] = obb.normaDirect[0] * obb.scale.x;
		otherDir[1] = obb.normaDirect[1] * obb.scale.y;
	}
	XMVECTOR p[4];
	p[0] = otherDir[0] + otherDir[1] + normal * normalScale + obb.center;
	p[1] = otherDir[0] - otherDir[1] + normal * normalScale + obb.center;
	p[2] = -otherDir[0] - otherDir[1] + normal * normalScale + obb.center;
	p[3] = -otherDir[0] + otherDir[1] + normal * normalScale + obb.center;


	//�E��荶���ǂ��炩�Ń��C��������ʂ��Ă�����OK
	bool check1 = true;
	bool check2 = true;

	XMVECTOR m;
	// ��p0_p1�ɂ���
	XMVECTOR pt_p0 = p[0] - resultInter;
	XMVECTOR p0_p1 = p[1] - p[0];
	m = XMVector3Cross(pt_p0, p0_p1);
	// �ӂ̊O��
	if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
		check1 = false;
	}

	if (check1)
	{
		// ��p1_p2�ɂ���
		XMVECTOR pt_p1 = p[1] - resultInter;
		XMVECTOR p1_p2 = p[2] - p[1];
		m = XMVector3Cross(pt_p1, p1_p2);
		// �ӂ̊O��
		if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
			check1 = false;
		}
	}
	if (check1)
	{
		// ��p2_p3�ɂ���
		XMVECTOR pt_p2 = p[2] - resultInter;
		XMVECTOR p2_p3 = p[3] - p[2];
		m = XMVector3Cross(pt_p2, p2_p3);
		// �ӂ̊O��
		if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
			check1 = false;
		}
	}
	if (check1)
	{
		// ��p3_p0�ɂ���
		XMVECTOR pt_p3 = p[3] - resultInter;
		XMVECTOR p3_p0 = p[0] - p[3];
		m = XMVector3Cross(pt_p3, p3_p0);
		// �ӂ̊O��
		if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
			check1 = false;
		}
	}
	if (!check1)
	{
		if (check2)
		{
			// ��p0_p1�ɂ���
			XMVECTOR pt_p0 = p[0] - resultInter;
			XMVECTOR p0_p1 = p[0] - p[1];
			m = XMVector3Cross(pt_p0, p0_p1);
			// �ӂ̊O��
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
		if (check2)
		{
			// ��p1_p2�ɂ���
			XMVECTOR pt_p1 = p[1] - resultInter;
			XMVECTOR p1_p2 = p[1] - p[2];
			m = XMVector3Cross(pt_p1, p1_p2);
			// �ӂ̊O��
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
		if (check2)
		{
			// ��p2_p3�ɂ���
			XMVECTOR pt_p2 = p[2] - resultInter;
			XMVECTOR p2_p3 = p[2] - p[3];
			m = XMVector3Cross(pt_p2, p2_p3);
			// �ӂ̊O��
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
		if (check2)
		{
			// ��p3_p0�ɂ���
			XMVECTOR pt_p3 = p[3] - resultInter;
			XMVECTOR p3_p0 = p[3] - p[0];
			m = XMVector3Cross(pt_p3, p3_p0);
			// �ӂ̊O��
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
	}
	//�E��荶�����ǂ�����O����ʂ��Ă����瓖�����Ă��Ȃ�
	if (!check1 && !check2)
		return false;
	
	if (inter) 
		*inter = resultInter;
	
	if (distance)
		*distance = resultDistance;

	// �����Ȃ̂ŁA�������Ă���
	return true;
}

// OBB v.s. OBB
bool Collision::CheckOBBOBB(const Box& box1, const Box& box2, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	
	auto scale1 = box1.scale;
	auto scale2 = box2.scale;

	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	Vector3 NAe1 = box1.normaDirect[0];
	Vector3 NAe2 = box1.normaDirect[1];
	Vector3 NAe3 = box1.normaDirect[2];
	Vector3 NBe1 = box2.normaDirect[0];
	Vector3 NBe2 = box2.normaDirect[1];
	Vector3 NBe3 = box2.normaDirect[2];
	Vector3 Ae1 = NAe1 * scale1.x;
	Vector3 Ae2 = NAe2 * scale1.y;
	Vector3 Ae3 = NAe3 * scale1.z;
	Vector3 Be1 = NBe1 * scale2.x;
	Vector3 Be2 = NBe2 * scale2.y;
	Vector3 Be3 = NBe3 * scale2.z;
	Vector3 Interval = box1.center - box2.center;

	float rejectLength = 0;
	Vector3 rejectVec = {};
	
	// ������ : Ae1
	float rA = Ae1.Length();
	float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	float L = fabs(Vector3::Dot(Interval, NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�
	
	// ������ : Ae2
	rA = Ae2.Length();
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe2));
	if (L > rA + rB)
		return false;
	
	// ������ : Ae3
	rA = Ae3.Length();
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.Length();
	L = fabs(Vector3::Dot(Interval, NBe1));
	if (L > rA + rB)
		return false;
	rejectLength = rB + rA - L;
	rejectVec = NBe1;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.Length();
	L = fabs(Vector3::Dot(Interval, NBe2));
	if (L > rA + rB)
		return false;
	if (rejectLength > rB + rA - L)
	{
		rejectLength = rB + rA - L;
		rejectVec = NBe2;
	}

	// ������ : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.Length();
	L = fabs(Vector3::Dot(Interval, NBe3));
	if (L > rA + rB)
		return false;
	if (rejectLength > rB + rA - L)
	{
		rejectLength = rB + rA - L;
		rejectVec = NBe3;
	}

	// ������ : C11
	Vector3 Cross;
	Cross = Vector3::Cross(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	Cross = Vector3::Cross(NAe1, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	Cross = Vector3::Cross(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	Cross = Vector3::Cross(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	Cross = Vector3::Cross(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	Cross = Vector3::Cross(NAe2, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	Cross = Vector3::Cross(NAe3, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	Cross = Vector3::Cross(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	Cross = Vector3::Cross(NAe3, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	if (reject)
	{
		//�r�˃x�N�g����Box1-2�̕����x�N�g�����t�����Ȃ�r�˃x�N�g�����t�����ɂ���
		float dot = rejectVec.Dot(Vector3::Normalize(Interval));
		if(dot < 0)
			rejectVec *= -1;
		
		*reject = Vector3(rejectVec * rejectLength).ConvertXMVECTOR();
	}
	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}
bool Collision::CheckOBBPlane(const Box& obb,const Plane& plane, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	// ���ʂ̖@���ɑ΂���OBB�̎ˉe���̒������Z�o
	float r = 0.0f;          // �ߐڋ���
	Vector3 PlaneNormal = plane.normal; // ���ʂ̖@���x�N�g��
	int i;
	for (i = 0; i < 3; i++) {

		float directLength = 0.0f;

		switch (i)
		{
		case 0:
			directLength = obb.scale.x;
			break;
		case 1:
			directLength = obb.scale.y;
			break;
		case 2:
			directLength = obb.scale.z;
			break;
		default:
			break;
		}
		
		Vector3 Direct = obb.normaDirect[i]; // OBB��1�̎��x�N�g��
		r += fabs(Vector3::Dot((Direct * directLength), PlaneNormal));
	}

	// ���ʂ�OBB�̋������Z�o
	Vector3 ObbPos = obb.center;
	Vector3 PlanePos = Vector3(0,0, plane.distance);
	float s = Vector3::Dot((ObbPos - PlanePos), PlaneNormal);

	// �Փ˔���
	if (fabs(s) - r >= 0.0f)
		return false; // �Փ˂��Ă��Ȃ�

	// �߂��������Z�o
	if (reject) {
		if (s > 0)
			*reject = Vector3(PlaneNormal * (r - fabs(s))).ConvertXMVECTOR();
		else
			*reject = Vector3(PlaneNormal * (r + fabs(s))).ConvertXMVECTOR();
	}

	return true; // �Փ˂��Ă���
}


float Collision::LenSegOnSeparateAxis(const Vector3& Sep, const Vector3& e1, const Vector3& e2, const Vector3& e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	float r1 = fabs(Vector3::Dot(Sep, e1));
	float r2 = fabs(Vector3::Dot(Sep, e2));
	float r3;
	if (e3.x != 0 || e3.y != 0 || e3.z != 0)
	{
		r3 = fabs(Vector3::Dot(Sep, e3));
	}
	else
		r3 = 0;
	return r1 + r2 + r3;
}

bool Collision::CheckSphereCapsule(const Sphere& sphere, const Capsule& capsule)
{
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	Vector3 n = vStartToEnd;
	n.Normalize();

	const float t = Vector3(sphere.center - capsule.startPosition).Dot(n);

	const Vector3 vPsPn = n * t;
	const Vector3 posPn = vPsPn + capsule.startPosition;

	const float lengthRate = t / vStartToEnd.Length();

	float distance;
	if (lengthRate < 0.0f)
	{
		distance = Vector3(sphere.center - capsule.startPosition).Length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		distance = Vector3(sphere.center - posPn).Length() - capsule.radius;
	}
	else
	{
		distance = Vector3(sphere.center - capsule.endPosition).Length() - capsule.radius;
	}
	if (sphere.radius > distance)
		return true;
	return false;
}

bool Collision::Detect(const Box& target,const Frustum& arg_frustum)
{
	Frustum frustum = arg_frustum;
	
	for (int i = 0; i < 6; i++)
	{
		Vector3 normal = frustum.GetPlanes().plane[i].normal;
		Vector3 vp = target.minPosition;

		if (normal.x > 0)
		{
			vp.x += target.scale.x;
		}
		if (normal.y > 0)
		{
			vp.y += target.scale.y;
		}
		if (normal.z > 0)
		{
			vp.z += target.scale.z;
		}

		Vector3 vn = target.minPosition;
		if (normal.x < 0)
		{
			vn.x += target.scale.x;
		}
		if (normal.y < 0)
		{
			vn.y += target.scale.y;
		}
		if (normal.z < 0)
		{
			vn.z += target.scale.z;
		}
		normal.Normalize();
		Vector3 planePos = frustum.GetPlanes().plane[i].pos;
		auto targetPos = vp - planePos;

		float dist = Vector3::Dot(targetPos, normal);
		if (dist > 0)//�_�����ʂɑ΂��ĕ\�ɂ���
		{
			continue;
		}
		targetPos = vn - planePos;
		dist = Vector3::Dot(targetPos, normal);
		if (dist > 0)//�_�����ʂɑ΂��ĕ\�ɂ���
		{
			continue;
		}
		return false;
	}
	return true;
}
