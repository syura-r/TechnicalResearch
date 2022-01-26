#pragma once
#include<DirectXMath.h>

#include "Camera.h"
#include"Vector.h"
struct Ray
{
	// �n�_
	DirectX::XMVECTOR	start = { 0,0,0,1 };
	// ����
	DirectX::XMVECTOR	dir = { 1,0,0,0 };
};

struct Plane
{
	// �@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0 };
	// ���_����̋���
	float distance = 0.0f;
};

struct Sphere
{
	// ���S���W
	DirectX::XMVECTOR center = {};
	// ���a
	float radius = 1.0f;
};

struct Capsule
{
	DirectX::XMVECTOR startPosition = {};
	DirectX::XMVECTOR endPosition = {};

	// ���a
	float radius = 1.0f;
};

struct Box
{
	//���S���W
	DirectX::XMVECTOR center = {};
	DirectX::XMVECTOR normaDirect[3] = {{},{},{}};
	//float fLength[3] = {};
	//�傫��
	Vector3 scale = { 1,1,1 };
	Vector3 minPosition = {};
	Vector3 maxPosition = {};
	Vector3 rotation = {};
};

class Triangle
{
public:
	// ���_���W3��
	DirectX::XMVECTOR	p0;
	DirectX::XMVECTOR	p1;
	DirectX::XMVECTOR	p2;
	// �@���x�N�g��
	DirectX::XMVECTOR	normal;

	/// <summary>
	/// �@���̌v�Z
	/// </summary>
	void ComputeNormal();
};

class Frustum
{
	struct FrustumPlane
	{
		Vector3 normal = { 0,0,0};
		Vector3 pos = { 0,0,0 };

	};
	struct FrustumPlanes
	{
		FrustumPlane plane[6];
	};
private:
	FrustumPlanes planes;

public:
	void CalculateFrustumPlanes(const Camera& camera);

	inline const FrustumPlanes& GetPlanes() { return planes; }

};
